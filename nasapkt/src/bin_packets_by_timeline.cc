/*****************************************************************************
*
* DESCRIPTION: This tool bins both head2 and head3 packets according
* according to a timeline. The tertirary header time is used for head3 packets.
*
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
#include "TimePacketBinner.h"
#include "Head3TimePacketBinner.h"



#include "PacketTee.h"
#include "PacketWriter.h"

#include <fstream>


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
ui->setDefault("errors", "errors");
ui->setDefault("obs_dir", "obs");
ui->setDefault("timeline","timeline");


{ // start of block to ensure destructors get called

/****************************************************
* make sure output directories exist
****************************************************/
File* obs_dir = new File(ui->getString("obs_dir").c_str(),"");
if( ! obs_dir->exists() ) {
    cerr << "Observation directory "<<obs_dir->name()<<" does not exist\n";
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
PacketWriter errors_writer(   ui->getOstream("errors"   , ios::app));

PacketTee trash;
PacketTee errors;

trash  |  trash_writer;
errors | errors_writer;

/********************
* read the timeline *
********************/
TimeLine* timeline = new TimeLine();

try { timeline->read(ui->getIstream("timeline")); }
catch(TimeInterval::Exception& e) {
    cerr << "Error in timeline "<<ui->getString("head2_timeline")<<"\n";
    cerr << e.what()<<"\n";
    exit(1);
}

/**********************************
* set up the  packet binning *
**********************************/
PacketTee head2_tee;
TimePacketBinner head2_binner(timeline);
Head3TimePacketBinner head3_binner(timeline);

head2_binner.dir(obs_dir->name());
head3_binner.dir(obs_dir->name());

head2_tee | head2_binner;


reader->head2() | head2_tee;
reader->tdrss() | head2_tee;
reader->head3() | head3_binner;

/************************************************
* we don't expect any of these kinds of packets *
************************************************/
reader->uvot()  | errors;
reader->acs()   | errors;
reader->ldp()   | errors;

/**********************************************************
* unclassifiable packets are errors 
* packets marked as trash are sent directly to the trash.
**********************************************************/
reader->generic() | errors;
reader->trash()   | trash;



/***********************************
* read and process all the packets *
***********************************/
reader->pump();

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
