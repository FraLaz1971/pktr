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
* - annotated If true, assume the packets have 12 byte ITOS annotation 
*   headers
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
ui->setDefault("version", 0);
ui->setDefault("telemetry_check",true);
ui->setDefault("command_check",false);
ui->setDefault("head2_check", true);
ui->setDefault("annotated", false);
ui->setDefault("npackets", 3);

/*************************************************************
* test to be sure the Writer is compatible with the hardware *
*************************************************************/
try {
    Reader::checkMachine();
} catch(Reader::HardwareException e) {
    cerr << "Hardware exception:" << e.what() << "\n";
    exit(1);
}

/******************
* read parameters *
******************/
unsigned int version = ui->getInteger("version");

bool check_telemetry = ui->getBoolean("telemetry_check");
bool check_command = false;
if(!check_telemetry) check_command = ui->getBoolean("command_check");

bool check_head2 = ui->getBoolean("head2_check");

bool annotated = ui->getBoolean("annotated");

CCSDSPacket* p = new CCSDSPacket();
ITOSAnnotation* itos = new ITOSAnnotation();
Reader* r = new Reader(ui->getIstream("infile"));

int npackets = ui->getInteger("npackets");

try {
    for(int i=0; i<npackets; ++i) {
        /*******************************************
        * read the packet, and possible annotation *
        *******************************************/
        if(annotated) itos->read(r);
        p->read(r);

        /***************************************************
        * check if there is anything wrong with the header *
        ***************************************************/
        if(p->version() != version)               exit(1);
        if(check_telemetry && p->type()     != 0) exit(1);
        if(check_command   && p->type()     != 1) exit(1);
        if(check_head2     && p->hasHead2() != 1) exit(1);


    }
} catch(Interpreter::EOFException e) {} // EOF is OK



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
