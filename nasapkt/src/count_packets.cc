/*****************************************************************************
*
* DESCRIPTION: This tool counts packets by APID and reports the results.
*
* PARAMETERS:
* - infile The file from which to read packets
* - outfile the file to which the count report is written. 
*
******************************************************************************/

#include "CommandLineUI.h"
#include "PacketReader.h"
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

/*
//istream* in = ui->getIstream("infile");
unsigned char buffer[20];

//streambuf* inbuf = new filebuf(stdin, ios::in, 1);

fread(buffer, 1, 20, stdin);

//istream* in = &cin;

//in->read((char*)buffer,20);
//in->rdbuf()->sgetn((char*)buffer, 20);
//inbuf->sgetn((char*)buffer, 20);

for(int i=0; i<20; ++i) {
    cout <<hex<<(int)buffer[i]<<"\n";
}

exit(1);
*/

/********************************
* create a reader and a counter *
********************************/
PacketReader reader(ui->getIstream("infile"), new CCSDSPacket());
PacketCounter counter;

reader | counter;

/************************************************************
* read and count all the packets and then print the results *
************************************************************/
counter.pump();
counter.report(ui->getOstream("outfile"));


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
