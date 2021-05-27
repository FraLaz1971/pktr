/*****************************************************************************
*
* DESCRIPTION: This tool read the tertiary header from an LDP header
* and outputs the proper filename for the LDP in the observation
* directory.
*
* PARAMETERS:
* - apid_list Configuration file identifying packets types by APID
* - dir Name of the observation directory to contain the renamed files
* - infile LDP file whose name we are trying to determine
* - outfile File to which the new filename willbe written (usually stdout).
*
******************************************************************************/

#include "CommandLineUI.h"
#include "SwiftTypedPacketReader.h"
#include "LDPBuffer.h"
#include "StandardLDPHead.h"
#include "DailyPacketBinner.h"


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

ui->setDefault("infile","-");
ui->setDefault("outfile","-");
ui->setDefault("apid_list","apids.list");
ui->setDefault("dir",".");


/*******************************************************************
* create a typed packet reader which will read the packets of the
* appropriate type
*******************************************************************/
SwiftTypedPacketReader reader(ui->getString("apid_list"),
                              ui->getIstream("infile")   );

/*****************************************************************
* create an LDPBuffer to read the ldp contents and hook it
* to the ldp output of the reader
*****************************************************************/
LDPBuffer* ldp=new LDPBuffer();

reader.ldp() | ldp->pipe();


/*******************************************************
* now wrap the ldp buffer in a stream and wrap that in 
* a Reader so we can read the header data.
********************************************************/
Reader* r = new Reader(new istream(ldp) );


/***********************************************************
* next read the header and check for errors
* any trouble with the LDP itself will cause the LDPBuffer
* to signal EOF, which will cause 
* the StandardLDPHead read method to throw an EOFException
***********************************************************/
StandardLDPHead head;

try {
    head.read(r);
} catch (Interpreter::Exception& e) {
    cerr << "Error reading ldp\n";
    exit(1);
}

/*********************************
* pull the data out of the header
*********************************/
SwiftHead3* head3 = head.head3();

/********************************************************
* check if the ID is set to the "unknown" value
* 0xffffffff If so we will use a timeline to generate
* the file name instead of the tertiary header
*******************************************************/


DailyPacketBinner binner(24*3600, 0.0, 5);


/***************************************************
* set the parts of the filename so that they are
* appropriate for LDPs
**************************************************/
binner.dir(ui->getString("dir"));
binner.base("swift_ldp_");

ostringstream tmp;
tmp << "_"<< ldp->product()<<".ccsds";
#ifdef FAKING_STRINGSTREAM
tmp << ends;
#endif
binner.ext(tmp.str());

/************************
* now get the file name *
************************/
File* file = binner.getFileFromTime(ldp->apid(), head3->sc_time()->value() );


ostream* out = ui->getOstream("outfile");

if(file == NULL) *out << "unknown"    << endl;
else             *out << file->name() << endl;




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
