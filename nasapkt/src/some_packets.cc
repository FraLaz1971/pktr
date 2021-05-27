/*****************************************************************************
*
* DESCRIPTION: This tool selects a subset of packets within a file. 
* For example, you can create a new file containing the 10th packet 
* through 100th packet in a given file.
*
* PARAMETERS:
* - infile Input file to read from. A "-" means read from stdin.
* - outfile Output file to write results to. A "-" means write to stdout.
* - from This is the number of the first packet to read. The first packet in  
*   the input stream is numbered "1".
* - to The number of the last packet to read from the input stream. A value
*   of "-1" means read to the end of the file.
*
******************************************************************************/

#include "PacketReader.h"
#include "PacketWriter.h"
#include "CommandLineUI.h"

void wrong_usage(const string& command) {
    cerr << "Usage "<<command<<" [from] - [to]\n";
    exit(1);
}


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

/*************
* parameters *
*************/
ui->setDefault("infile", "-");
ui->setDefault("outfile", "-");
ui->setDefault("from", 1);
ui->setDefault("to", -1);

int from = ui->getInteger("from");
int to   = ui->getInteger("to");


PacketReader reader(ui->getIstream("infile"), new CCSDSPacket() );
PacketWriter writer(ui->getOstream("outfile") );
CCSDSPacket* p;

/*********************
* skip the beginning *
*********************/
int i=1;
for( ; i<from; ++i) {
    reader >> p;
}


/*****************************
* copy the requested packets *
*****************************/
for( ; !(to == -1 && p ==NULL) && !(to != -1 && i>to); ++i) {

    reader >> p;
    writer << p;
}


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

    



