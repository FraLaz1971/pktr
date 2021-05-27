/*****************************************************************************
*
* DESCRIPTION: 
*
* PARAMETERS:
*
******************************************************************************/

#include <iomanip>
#include "CommandLineUI.h"
#include "SwiftTypedPacketReader.h"
#include "PacketTee.h"
#include "CCSDSMerger.h"
#include "PacketTank.h"

#include "FlowProbe.h"


/*****************************************************
* this is a small class for storing an obs/target ID *
*****************************************************/
class ID {

public:

bool warned;
bool defined;
int target;
int obs;

ID() { 
    defined=false; 
    warned = false;
}

void set(int target, int obs) {
    if(! defined) {
        this->target=target;
        this->obs=obs;
        defined=true;
        return;
    }
        
    if(! warned && (this->target != target || this->obs != obs) ) {
        cerr << "multiple IDs: target="<<target<<" obs="<<obs<<"\n";
        warned=true;
    }

} // end of set method

unsigned int get() {
    unsigned int id;
    
    id = (target << 8) | obs;

    return id;
}

}; // end of ID class;


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
ui->setDefault("apid_list","/aps/lists/swift_apids.list");


{ // local block so destructors are called before exiting

FlowProbe probe;


/************************
* create a typed reader *
************************/
SwiftTypedPacketReader reader(ui->getString("apid_list"), 
                              ui->getIstream("infile"));

/******************************************************
* tee all the reader output channels together
* we will later use dynamic casts to determine the
* packet types
*****************************************************/
PacketTee tee;

reader.head2()    | tee;
reader.head3()    | tee;
reader.ldp()      | tee;
reader.acs()      | tee;

/********************************************************************
* we have to add a "tank" buffer after the tee because
* pulling from the tee means pulling from a random channel
* of the reader, which can result in packets coming out of 
* it's other channels before the pulled channel produces anything
*******************************************************************/
PacketTank tank;
tee | tank;

/*********************************************************
* we want to merge the UVOT packets so that we can read
* the IDs from the resulting HEAD3 packets 
*********************************************************/
CCSDSMerger merger;
merger.setNewAPID(768,855);
merger.setNewAPID(770,857);

reader.uvot() | merger | tee;

/*************************************************************************
* now we will read all the packets by pulling them one at a time
* from the tank. This could be done more efficiently be writing
* a special pipe class for the task, but who needs all that efficiency
* anyway?
*************************************************************************/
CCSDSPacket* p = NULL;

double start=0.0;
double stop=0.0;
bool first=true;

ID id;

while(true) {

    /***********************
    * read the next packet *
    ***********************/
//cout << "about to read packet\n";
    tank >> p;
//cout << "done reading packet\n\n";
    if(p==NULL) break;

    /************************************************
    * determine the earliest and latest head2 times *
    ************************************************/
    SwiftPacket* head2 = dynamic_cast<SwiftPacket*>(p);
    if(head2!=NULL) {

        if(first || head2->time() <start) start=head2->time(); 
        if(first || head2->time() >stop ) stop =head2->time();
        if(first) first=false;
    }

    /******************************
    * head3 packets - read the ID *
    ******************************/
    SwiftHead3Packet* head3 = dynamic_cast<SwiftHead3Packet*>(p);
    if(head3!=NULL) {

        id.set((int)(head3->target()),(int)(head3->obs()) );
    }




    
    

} // end of loop over packets

/******************************
* print the results to stdout *
******************************/
if(id.defined) {
    cout << setprecision(14) << start <<" "<<stop
         <<" "<< id.get()
         <<"\n";
} else {
    cout << setprecision(14) << start <<" "<<stop
         <<"\n";
}



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
