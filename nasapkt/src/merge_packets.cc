/****************************************************************************
* DESCRIPTION: This is a very simple program for merging UVOT segmented
* packets. It is designed to handle an input stream containing only packets
* with APID=768, from which it will produce a merged stream of packets
* with APID=769. This will be replaced with a more sophisticated tool when
* I have an opportunity to work with more complicated test data from the 
* UVOT team.
*
* PARAMETERS:
* - infile The input file containing segmented packets with APID=768.
* - outfile The output file which will contain the merged packets with
*   APID=769.
****************************************************************************/
#include <fstream>
#include "PacketReader.h"
#include "CCSDSMerger.h"
#include "PacketWriter.h"
#include "SwiftHead3Packet.h"
#include "CommandLineUI.h"

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


int main(int argc, char** argv) {

/***********************************
* set emergency exception handlers *
***********************************/
set_unexpected(my_unexpected);
set_terminate(my_terminate);


try { // big try block around the entire program
UserInterface* ui = new CommandLineUI(argc, argv);

/*************
* parameters *
*************/
ui->setDefault("infile", "-");
ui->setDefault("outfile", "-");




PacketReader reader(ui->getIstream("infile"), new SwiftPacket() );
CCSDSMerger merger;
merger.setNewAPID(768, 769);

PacketWriter writer(ui->getOstream("outfile") );

PacketPipe pipe;

reader | pipe | merger | writer;

pipe.pump();

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
