/*****************************************************************************
*
* DESCRIPTION: This is a standalone tool for updating a timeline from
* a list of ACS packets. This function will usually be done by bin_packets.
*
* PARAMETERS:
*
* - infile The file containing ACS packets
*
* - timeline The file containing the timeline. This file will be overwritten 
*   with the updated timeline, so you might want to savbe a copy of the original
*
* - gap Gaps in the timeline between adjacent intervals with the same ID 
*       which are shorter than this tolerance will be filled in.
*
******************************************************************************/

#include "CommandLineUI.h"

#include "PacketReader.h"
#include "ACSTimeLineUpdater.h"
#include "SwiftPacket.h"

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

ui->setDefault("infile","/usr/home/pier/swift/telemetry/bat/acs/acspkts9_23.0");
ui->setDefault("timeline","ztime");
ui->setDefault("gap", 100);

/*************************************************************
* test to be sure the Writer is compatible with the hardware *
*************************************************************/
try {
    Writer::checkMachine();
} catch(Reader::HardwareException e) {
    cerr << "Hardware exception:" << e.what() << "\n";
    exit(1);
}

{ // local block so destructors are called before exiting

/*********************************************
* create a packet reader to read ACS packets *
*********************************************/
PacketReader reader(ui->getIstream("infile"), new SwiftPacket() );

/******************************************************
* create a timeline and read its contents from a file *
******************************************************/
TimeLine* timeline = new TimeLine();
timeline->read(ui->getIstream("timeline") );

ACSTimeLineUpdater updater(timeline);

/***************************************************
* link the reader to the updater and run the chain *
***************************************************/
reader | updater;
updater.pump();

/*******************************************
* fill in what gaps we can in the timeline *
*******************************************/
try { timeline->fillGaps(ui->getReal("gap")); } 
catch(TimeInterval::Exception& e) {
    /**************************
    * this should never occur *
    **************************/
    cerr <<"Error filling gaps in timeline: "<<e.what()<<"\n";
    exit(1);
}

/***********************************************************
* now write the timeline back over the file we read it from *
***********************************************************/
timeline->write(ui->getOstream("timeline"));

/*************************************************
* see if we had any errors updating the timeline *
*************************************************/
if(updater.getErrorCount()) exit(1);


} // end of local block so destructors are called before exiting
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
