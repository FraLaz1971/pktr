/*****************************************************************************
*
* DESCRIPTION: This tool sorts a stream of CCSDS packets
* assuming they are all Swift "head2" packets. This means that they will
* be sorted by secondary header time. 
*
* PARAMETERS:
*
* - infile The input file name. 
* - outfile the output file name. The output file should not be the
*   same as the input file.
* - apid_list The name of a configuration file for the SwiftTypedPacketReader
*   which lists the packet types for various APIDs.
* - unique Should the packets be "uniqued" after sorting?
* - trash The name of a file to which packets deleted by uniquing will be
*   appended. If this is an empty string, deleted packets will be discarded.
*   This parameter is ignored if the "unique" parameter is false.
*
******************************************************************************/

#include "CommandLineUI.h"
#include "PacketReader.h"
#include "SwiftPacket.h"
#include "PacketTee.h"
#include "PacketTank.h"
#include "PacketWriter.h"

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

ui->setDefault("infile", "-");
ui->setDefault("outfile", "-");


/***************************
* create the packet reader *
***************************/
PacketReader reader(ui->getIstream("infile"), new SwiftPacket() );

/***************************************************
* now connect up a tank and a writer 
* we use a straight section of pipe after the tank
* for pumping the tank dry after sorting
***************************************************/
PacketTank tank;
PacketPipe pipe1;
PacketPipe pipe2;
PacketWriter writer(ui->getOstream("outfile"));

reader | pipe1 | tank | pipe2 | writer;


/****************************************************************
* fill the tank, sort the packets there and then flush the tank *
****************************************************************/
pipe1.pump();
tank.sort();
pipe2.pump();



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
