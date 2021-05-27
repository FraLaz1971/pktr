/*****************************************************************************
*
* DESCRIPTION: This tool sets the Swift obs/target IDs in Swift telemetry.
* It adjusts the values in packets with tertiary headers as well as
* in the first page of an LDP. 
* Note this tool will not work on UVOT segmented packets. Such packets
* must first be merged. Note also that it only adjusts the values in headers.
* In particular it does not touch the obs/target ID embedded in the data of
* ACS packets
*
* PARAMETERS:
* - infile File containing CCSDS telemetry to be updated.
*
* - outfile File which will contain the modified telemetry
*
* - apid_list The name of a SwiftTypedPacketReader configuration file, 
*   listing packet types by APID
*
* - 24_target The 24 bit target ID to use
*
* - 8_obs The 8 bit obs ID to use.
*
******************************************************************************/

#include "CommandLineUI.h"

#include "SwiftTypedPacketReader.h"
#include "SwiftIDSetter.h"
#include "SwiftIDCounter.h"
#include "PacketTee.h"
#include "PacketWriter.h"
#include "PacketCounter.h"


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

ui->setDefault("infile","-");
ui->setDefault("outfile","-");
ui->setDefault("apid_list", "swift_apids.list");
ui->setDefault("24_target", 0);
ui->setDefault("8_obs", 1);

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


/*************************************************************
* create a SiwftTypedPacketReader which will read the
* packets and create objects of the correct types for them
*************************************************************/
SwiftTypedPacketReader reader(ui->getString("apid_list"),
                              ui->getIstream("infile")    );

/********************************************
* create the SwiftIDSetter and configure it *
********************************************/
SwiftIDSetter setter;
setter.setObs(   (unsigned char)(ui->getInteger("8_obs")    ) );
setter.setTarget((unsigned int )(ui->getInteger("24_target")) );

SwiftIDCounter ticker;

/****************
* misc plumbing *
****************/
PacketWriter writer(ui->getOstream("outfile"));
PacketTee modifiable;
PacketTee all;
PacketCounter counter;

/*******************************
* now link everything together *
*******************************/
reader.head3() | modifiable;
reader.ldp()   | modifiable;

reader.uvot()  | all;
reader.head2() | all;
reader.acs()   | all;

reader.generic() | counter | all;

modifiable | ticker | setter |all | writer;

/*********
* run it *
*********/
cout << "The following IDs were found in the input telemetry\n";
reader.pump();

/*******************************************
* check if we had any unidentified packets *
*******************************************/
if(counter.getCount()) {
    cerr << "\n";
    cerr << "Error: Could not determine the types of the following packets:\n";
    counter.report(&cerr);
}


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
