/*****************************************************************************
*
* DESCRIPTION: This tool checks to see if a file contains plain, unannotated
* CCSDS telemetry packets. It does this by attempting to read the first few 
* packets from a file and checking header values like the version field.
* It exits with status 0 if the fiel appears to contain CCSDS telemetry,
* and exits with status 1 if the file does not. Status > 1 indicates an error.
*
* PARAMETERS:
* - infile The file to be checked.
*
* - npackets Number of packets to verify at the beginning of the file.
*   If the file contains fewer than this number of packets, it is not
*   considered an error. An empty file will be called CCSDS.
*
* - version The value of the CCSDS version number to expect. Currently, this
*   should always be the default, 0.
*
* - telemetry_check Check that the packet type bit is set to "telemetry"
*
* - command_check Check that the packet type bit is set to "command".
*   This parameter is ignored if telemetry_check=true
*
* - head2_check Check that the header indicates the packet contains a 
*   secondary header.
*
******************************************************************************/

#include "CommandLineUI.h"
#include "Reader.h"
#include "CCSDSPacket.h"
#include "ITOSAnnotation.h"


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

/*************************************************************
* test to be sure the Writer is compatible with the hardware *
*************************************************************/
try {
    Reader::checkMachine();
} catch(Reader::HardwareException e) {
    cerr << "Hardware exception:" << e.what() << "\n";
    exit(1);
}


CCSDSPacket* p = new CCSDSPacket();
ITOSAnnotation* itos = new ITOSAnnotation();
Reader* r = new Reader(ui->getIstream("infile"));
Writer* w = new Writer(ui->getOstream("outfile"));

/**********************************
* read packets until we reach EOF *
**********************************/
try {
    for(int i=0; ;++i) {

        itos->read(r);
        p->read(r);

/*

cerr << "apid="<<p->apid() << " "
     << itos->rs_error() <<" "
     << itos->head_error() <<" "
     << itos->frame_error() <<" "
     << itos->incomplete() <<" "
     << itos->frame_head_error() <<" "
     <<"\n";
*/


        if(itos->rs_error() ) {
            cerr << "Reed Solomon Error in packet "
                 << i <<" APID "<<p->apid()<<"\n";

        } else if(itos->head_error() ) {
            cerr << "Packet header from bad frame set "
                 << i <<" APID "<<p->apid()<<"\n";

        } else if(itos->frame_error() ) {
            cerr << "Error in parent frame "
                 << i <<" APID "<<p->apid()<<"\n";

        } else if(itos->incomplete() ) {
            cerr << "Packet incomplete "
                 << i <<" APID "<<p->apid()<<"\n";

        } else if(itos->incomplete() ) {
            cerr << "Incorrect version or Spacecraft ID in frame header "
                 << i <<" APID "<<p->apid()<<"\n";

        } else {
            /**************
            * good packet *
            **************/
            p->write(w);
        }

        
        

    }
} catch(Interpreter::EOFException e) {}


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
