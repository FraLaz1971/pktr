/*****************************************************************************
*
* DESCRIPTION: This tool does a first cut at binning Swift packets by
* observation. It first divides packets into 4 general types, depending on
* their APID. Each type is binned in a different way.
*
* - HEAD3 packets contain the target/obs ID in their tertiary header
*
* - HEAD2 packets contain no target/obs ID, so we much use a timeline to 
*   determine this info
*
* - UVOT packets are segmented and have one tertiary header per group.
*   We must therefore recombine the segments before extracting the
*   target/obs ID.
*
* - LDP packets similarly contain one tertiary header per "large data product".
*   However, we cannot merge them on the fly as we do UVOT packets.
*   Therefore LDPs are binned by product number and time. The resulting files
*   require further processing to get them into the correct observation bin.
*
* In all cases, different APIDs are segregated into different files. Bins
* are represented by directories.
*
*
* PARAMETERS:
*
* - infile The file containing packets to be sorted
*
* - apid_list SwiftTypedPacketReader configuration file, listing packet types 
*   by APID
*
* - errors File to which will be appended packets which cannot
*   be classified, even with possible future information. Under normal
*   conditions nothing will be written to this file.
*
* - pushback File to which will be appended packets which cannot
*   be sorted now, but may be sortable in the future. These include 
*   HEAD2 packets not covered by the current timeline and UVOT science
*   packets which are not part of complete groups
*
* - trash File to which will be appended discarded packets, such
*   as duplicate UVOT packets.
*
* - uvot_used File to which will be apppended UVOT science
*   packets which have been sucessfully merged. Note these are the original
*   unmerged packets.
*
* - head2_timeline File containing a timeline to use to sort HEAD2 packets
*   by observation.
*
* - sdc_timeline File containing a timeline which will be updated by the
*   information in the telemetry. Currently only the ACS packets are used to 
*   update the timeline. This parameter is not used to bin HEAD2 packets,
*   unless sdc_timeline and head2_timeline have the same value. In that case
*   the packets will be binned using the timeline as it was when this
*   tool was started. In other words the updates are not applied until this
*   tool exits sucessfully.
*
* - gap Size of the largest gap which will be automatically filled in the
*   sdc_timeline in seconds. Only gaps between adjacent intervals with the
*   same ID will be filled.
*
* - ldp_dir Directory where LDP packets will be written.
*
* - obs_dir Directory where non-LDP packets will be written.
*
******************************************************************************/

#include "CommandLineUI.h"
#include "SwiftTypedPacketReader.h"
#include "TimeLine.h"
// #include "TimePacketBinner.h"
#include "DailyPacketBinner.h"
#include "CCSDSMerger.h"

#include "LDPChecksumFilter.h"
#include "APIDFilter.h"

#include "Head3PacketBinner.h"
#include "ACSPacketBinner.h"
#include "LDPStubPacketBinner.h"
#include "TDRSSPacketBinner.h"

#include "ACSTimeLineUpdater.h"

#include "PacketWriter.h"
#include "PacketDeleter.h"
#include <fstream>

#include "TimeFilter.h"


#include "FlowProbe.h"


/*******************************
* emergency exception handlers *
*******************************/
void my_unexpected() {
    cerr << "Unexpected exception thrown\n";
    exit(1);
}

void my_terminate() {
    cerr << "Terminating - probably from uncaught exception\n";
    exit(1);
}


/***************************************************************************
*
***************************************************************************/
int main(int argc, char** argv) {

/***********************************
* set emergency exception handlers *
***********************************/
set_unexpected(my_unexpected);
set_terminate(my_terminate);


try { // big try block around the entire program
UserInterface* ui = new CommandLineUI(argc, argv);

ui->setDefault("apid_list", "apids.list");
ui->setDefault("infile", "-");
ui->setDefault("trash", "trash");
ui->setDefault("pushback", "pushback");
ui->setDefault("errors", "errors");
ui->setDefault("uvot_used", "uvot_used");
//ui->setDefault("head2_timeline", "timeline");
ui->setDefault("sdc_timeline","timeline");
ui->setDefault("gap",100.);
ui->setDefault("last_crack_time", 0.0);
ui->setDefault("ldp_dir", "ldp");
ui->setDefault("obs_dir", "obs");
ui->setDefault("first_bat_apid",289);
ui->setDefault("last_bat_apid",571);
ui->setDefault("highest_apid",2045);
ui->setDefault("chunk_time", 3600);
ui->setDefault("ldp_time", 86400);
ui->setDefault("digits", 5);
ui->setDefault("time_chunk_dir", "daily");
ui->setDefault("min_time", 0.0);
ui->setDefault("max_time", 1.0e99);

//cout << "bin_packets: start\n";

{ // start of block to ensure destructors get called

/****************************************************
* make sure output directories exist
****************************************************/
File* obs_dir = new File(ui->getString("obs_dir").c_str(),"");
if( ! obs_dir->exists() ) {
    cerr << "Observation directory "<<obs_dir->name()<<" does not exist\n";
    exit(1);
}

File* ldp_dir = new File(ui->getString("ldp_dir").c_str(),"");
if( ! ldp_dir->exists() ) {
    cerr << "LDP directory "<<ldp_dir->name()<<" does not exist\n";
    exit(1);
}


File* chunk_dir = new File(ui->getString("time_chunk_dir").c_str(),"");
if( ! chunk_dir->exists() ) {
    cerr << "Time-chunked directory "<<chunk_dir->name()<<" does not exist\n";
    exit(1);
}


/*********************************
* create the typed packet reader *
*********************************/
const string& config= ui->getString("apid_list");
SwiftTypedPacketReader* reader = new SwiftTypedPacketReader(config, 
                                                      ui->getIstream("infile"));


/*****************************
* create special recepticals *
*****************************/
PacketWriter trash_writer(    ui->getOstream("trash"    , ios::app));
PacketWriter pushback_writer( ui->getOstream("pushback" , ios::app));
PacketWriter errors_writer(   ui->getOstream("errors"   , ios::app));
PacketWriter uvot_used_writer(ui->getOstream("uvot_used", ios::app));

PacketTee trash;
PacketTee pushback;
PacketTee errors;
PacketTee uvot_used;

/******************************************************************
* note we have to delete the UVOT segments since they get
* uniqued by the merger. A few segmented packets will stray into
* some of the other recepticals, but we catch most of them by
* deleting the output of uvot_used_writer.
******************************************************************/
PacketDeleter segmented_deleter;

trash     |     trash_writer;
pushback  |  pushback_writer;
errors    |    errors_writer;
uvot_used | uvot_used_writer | segmented_deleter;

/***********************************************************
* bin HEAD2 packets by time interval
* anything which can't be binned goes to the pushback file
***********************************************************/
// TimeLine* timeline = new TimeLine();
// 
// try { timeline->read(ui->getIstream("head2_timeline")); }
// catch(TimeInterval::Exception& e) {
//     cerr << "Error in timeline "<<ui->getString("head2_timeline")<<"\n";
//     cerr << e.what()<<"\n";
//     exit(1);
// }

// TimePacketBinner time_binner(timeline);
// time_binner.dir(obs_dir->name() );

DailyPacketBinner daily_binner(ui->getReal("chunk_time"),
                               0.0,
                               ui->getInteger("digits"));
daily_binner.dir(chunk_dir->name() );


PacketTee head2_tee;
TimeFilter head2_time_filter(ui->getReal("min_time"), ui->getReal("max_time") );

reader->head2() | head2_time_filter | head2_tee | daily_binner | errors;


/*****************************
* merge UVOT science packets *
*****************************/
CCSDSMerger merger;
merger.setNewAPID(688, 843); // GeNIe data
merger.setNewAPID(704, 853); // finding chart
merger.setNewAPID(768, 855); // uncompressed image
merger.setNewAPID(769, 856); // compressed image
merger.setNewAPID(770, 857); // uncompressed events
merger.setNewAPID(771, 858); // compressed events
merger.setNewAPID(774, 852); // raw event engineering data
merger.setNewAPID(780, 854); // centroid confirmation

merger.scraps()     | pushback;
merger.duplicates() | trash;
merger.used()       | uvot_used;

/****************
* HEAD3 packets *
****************/
PacketTee head3_tee;
TimeFilter head3_time_filter(ui->getReal("min_time"), ui->getReal("max_time") );
TimeFilter  uvot_time_filter(ui->getReal("min_time"), ui->getReal("max_time") );

Head3PacketBinner head3_binner;
head3_binner.dir(obs_dir->name() );

PacketTee  apid_filter_tee;

APIDFilter is_bat_filter;
is_bat_filter.addInterval(ui->getInteger("first_bat_apid"),
                          ui->getInteger( "last_bat_apid") );


APIDFilter not_bat_filter;
not_bat_filter.addInterval(0, ui->getInteger("first_bat_apid")-1);

not_bat_filter.addInterval(ui->getInteger("last_bat_apid")+1,
                           ui->getInteger("highest_apid")   );


reader->uvot()  |  uvot_time_filter | merger                           | head3_tee;
reader->head3() | head3_time_filter | apid_filter_tee | not_bat_filter | head3_tee | head3_binner | errors;
                                      apid_filter_tee |  is_bat_filter | head2_tee;

/**************
* ACS packets *
**************/
TimeLine* sdc_timeline = new TimeLine();
sdc_timeline->read(ui->getIstream("sdc_timeline") );
ACSTimeLineUpdater timeline_updater(sdc_timeline);


ACSPacketBinner acs_binner;
acs_binner.dir(obs_dir->name() );

TimeFilter  acs_time_filter(ui->getReal("min_time"), ui->getReal("max_time") );

reader->acs() | acs_time_filter | timeline_updater | acs_binner | errors;

/************************
* Special TDRSS packets *
************************/
TDRSSPacketBinner tdrss_binner;
tdrss_binner.dir(obs_dir->name() );
reader->tdrss() | tdrss_binner | errors;

/*******************************************
* LDP packets get binned by product number *
*******************************************/
LDPChecksumFilter ldp_checksum_filter;
LDPStubPacketBinner ldp_binner;
ldp_binner.dir(ldp_dir->name());
ldp_binner.initRepositories();

TimeFilter  ldp_time_filter(ui->getReal("min_time"), ui->getReal("max_time") );

reader->ldp() | ldp_time_filter | ldp_checksum_filter | ldp_binner | pushback;


/**********************************************************
* unclassifiable packets are errors 
* packets marked as trash are sent directly to the trash.
**********************************************************/
reader->generic() | errors;
reader->trash()   | trash;



/***********************************
* read and process all the packets *
***********************************/
cout << "pumping"<<endl;
reader->pump();
cout << "done"<<endl;

/*******************************************
* fill in what gaps we can in the timeline *
*******************************************/
try { 
    sdc_timeline->fillGaps(ui->getReal("gap"));
    sdc_timeline->splitGaps(ui->getReal("gap"), ui->getReal("last_crack_time"));
} catch(TimeInterval::Exception& e) {
    /**************************
    * this should never occur *
    **************************/
    cerr <<"Error filling gaps in timeline: "<<e.what()<<"\n";
    exit(1);
}

/************************************************************
* now write the timeline back over the file we read it from *
************************************************************/
sdc_timeline->write(ui->getOstream("sdc_timeline"));

} // end of block to ensure destructors get called

/******
* end *
******/
delete ui;
exit(0);

} catch(UserInterface::Exception& e) {
    cerr << "User Interface Exception:\n";
    cerr << e.what()<<"\n";
    exit(1);
} catch(Interpreter::Exception& e) {
    cerr << "I/O Exception:\n";
    cerr << e.what()<<"\n";
    exit(1);
}


} // end of main
