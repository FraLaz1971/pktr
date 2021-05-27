/*****************************************************************************
*
* DESCRIPTION: This tool deletes packets with bad LDP checksums from a data 
* stream
*
*
* PARAMETERS:
*
* - infile The file containing packets to be sorted
*
* - apid_list SwiftTypedPacketReader configuration file, listing packet types 
*   by APID
*

*
******************************************************************************/

#include "CommandLineUI.h"
#include "SwiftTypedPacketReader.h"
#include "PacketTee.h"
#include "LDPChecksumFilter.h"
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

ui->setDefault("apid_list", "/aps/lists/swift_apids.list");
ui->setDefault("infile", "-");
ui->setDefault("outfile", "-");


{ // start of block to ensure destructors get called



/*********************************
* create the typed packet reader *
*********************************/
const string& config= ui->getString("apid_list");
SwiftTypedPacketReader* reader = new SwiftTypedPacketReader(config, 
                                                      ui->getIstream("infile"));
                                                    
LDPChecksumFilter filter;
PacketWriter writer(ui->getOstream("outfile"));
                                                      
//PacketTee tee;

reader->ldp() | filter | writer;

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
