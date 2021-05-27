/*****************************************************************************
* DESCRIPTION: This tool reads the standard header from a file containing 
* a single LDP and writes the contents in human-readable form to stdout.

*
* PARAMETERS:
* - apid_list Configuration file identifying packet types by APID
* - infile The file containing the LDP
*
******************************************************************************/

#include "CommandLineUI.h"
#include "SwiftTypedPacketReader.h"
#include "LDPBuffer.h"
#include "StandardLDPHead.h"

#include <iomanip>

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
ui->setDefault("apid_list","/aps/lists/swift_apids.list");


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

cout << setprecision(14);
cout << "NPAGES="  <<head.npages()               <<endl;
cout << "OBS="     <<head3->obs()                <<endl;
cout << "TARGET="  <<head3->target()             <<endl;
cout << "SC_TIME=" <<head3->sc_time()->value()   <<endl;
cout << "UTCF="    <<head3->utcf_time()->value() <<endl;

/***********************************************************************
* now read the secondary header from the first packet after the header *
***********************************************************************/
CCSDSPacket* generic_p = NULL;
reader.ldp() >> generic_p;
LDPPacket* p = dynamic_cast<LDPPacket*>(generic_p);

if(p!= NULL) {
    cout << "NEXT_PAGE="<<p->page()<<endl;
    cout << "HEAD2_TIME="<<p->time()<<endl;
} else {
    cout << "NEXT_PAGE=none"<<endl;
    cout << "HEAD2_TIME=\"\""<<endl;
}

/************************************
* determine the best time available *
************************************/
if(head3->sc_time()->seconds() == 0 ||
   head3->sc_time()->seconds() == 0xffffffff ) {
    /*********************************
    * the LDP header time is unreliable *
    ************************************/
    if(p!=NULL && p->page() == 2) {
        /******************************************************
        * we can use the secondary header time from the page2 *
        ******************************************************/
        cout << "BEST_TIME="<<p->time()<<endl;
    } else {
        /****************************
        * There is no reliable time *
        ****************************/
        cout << "BEST_TIME=\"\""<<endl;
    }
} else {
    /***************************
    * the tertiary header time is reliable *
    ***************************************/
    cout << "BEST_TIME="<<head3->sc_time()->value()<<endl;
}




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
