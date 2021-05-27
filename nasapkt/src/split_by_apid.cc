/*****************************************************************************
*
* DESCRIPTION: Splits a stream of packets by APID, writing each APID to a
* different file. If the output files exist, then this tool will append 
* to them
*
* PARAMETERS:
*
* - infile The file containing packets to be sorted
*
* - format a C printf format specification of the filename, with the APID
*   given as an integer argument.
*
* - dir the directory where the output files shoudl be written
*
******************************************************************************/

#include "CommandLineUI.h"
#include "PacketReader.h"
#include "APIDPacketBinner.h"


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
ui->setDefault("format","apid_0x%03x.ccsds");
ui->setDefault("dir", ".");


{ // local block so destructors are called before exiting


/****************************
* initialize the components *
****************************/
PacketReader reader(ui->getIstream("infile"), new CCSDSPacket() );
APIDPacketBinner binner(ui->getString("format"));

binner.dir(ui->getString("dir") );

/*********************************************
* connect everything together and make it go *
*********************************************/
reader | binner;
reader.pump();


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
