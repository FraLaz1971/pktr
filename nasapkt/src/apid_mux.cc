/*****************************************************************************
*
* DESCRIPTION: This tool "bins" packets by apid.  No heed is paid to
* the time or obsid.  It simply multiplexes one input stream into many
* output files, one for each apid.  The file names are of the form
*
*            apid_0x<apid>.ccsds
*
* PARAMETERS:
*
* - infile The file containing packets to be sorted
*
* - apid_list SwiftTypedPacketReader configuration file, listing packet types 
*   by APID.  If "all" is specified, then all packets types are processed.
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
*   as those which fail the apid_list test.  A value of "none" means
*   packets should be discarded instead of being sent to the trash stream.
*
* - obs_dir Directory where non-LDP packets will be written.
*
******************************************************************************/

#include "CommandLineUI.h"
#include "SwiftTypedPacketReader.h"
#include "APIDPacketBinner.h"
#include "CCSDSMerger.h"

#include "PacketWriter.h"
#include "PacketDeleter.h"
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

ui->setDefault("apid_list", "all");
ui->setDefault("infile", "-");
ui->setDefault("trash", "none");
ui->setDefault("pushback", "pushback");
ui->setDefault("errors", "errors");
ui->setDefault("gap",100.);
ui->setDefault("obs_dir", "obs");

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

/*********************************
* create the typed packet reader *
*********************************/
const string& config= ui->getString("apid_list");
SwiftTypedPacketReader* reader;

 if (config == "all") {
    reader = new SwiftTypedPacketReader("", ui->getIstream("infile"));
 } else {
    reader = new SwiftTypedPacketReader(config, ui->getIstream("infile"));
 }

/*****************************
* create special recepticals *
*****************************/
PacketWriter trash_writer(    ui->getOstream("trash"    , ios::app));
PacketWriter pushback_writer( ui->getOstream("pushback" , ios::app));

PacketTee pushback;
PacketTee trash;
pushback  |  pushback_writer;
trash     |  trash_writer;

APIDPacketBinner apid_binner("apid_0x%03x.ccsds");
apid_binner.dir(obs_dir->name() );

 if (config == "all") {
   /* If the user wanted everything ("all" apids), then tap the
      generic spigot */
   reader->generic() | apid_binner;
 } else {
   /* If the user specified explicit apids, then tap each kind  */
   reader->head2() | apid_binner;
   reader->head3() | apid_binner;
   reader->uvot()  | apid_binner;
   reader->ldp()   | apid_binner;
   reader->acs()   | apid_binner;
   reader->tdrss() | apid_binner;

   if (ui->getString("trash") != "none") {
     reader->generic() | trash;
   }
 }

 /* Who knows what this does, but do it */
 apid_binner | pushback;


/***********************************
* read and process all the packets *
***********************************/
cout << "pumping"<<endl;
reader->pump();
cout << "done"<<endl;



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
