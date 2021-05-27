/*****************************************************************************
*
* DESCRIPTION: This tool determines the proper filename for the LDP in the 
* observation directory for a given LDP. This can happen in one of several 
* ways. First, for NFI LDPS we use the ID from the tertiary header
* embedded in the LDP header. Next, for BAT LDPs and LDPs which have the 
* ID set to 0xffffffff, we use the time from the tertiary header in 
* conjunction with a timeline. BAT and the NFIs use different timelines.
* If the tertiary header time is set to an invalid value, then we use the 
* secondary header from page 2 of the LDP. Finally, for LDPs which are old
* enough that we are confdent that no more packets will arive, we use the 
* secondary header time from any available packet.
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
#include "Head3PacketBinner.h"
#include "TimePacketBinner.h"
#include "TimeLine.h"

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
ui->setDefault("index",0);
ui->setDefault("apid_list","apids.list");
ui->setDefault("dir",".");
ui->setDefault("first_bat_apid",289);
ui->setDefault("last_bat_apid",571);
ui->setDefault("timeline", "timeline");
ui->setDefault("bat_timeline", "bat_timeline");
ui->setDefault("mature_data_time", 0.0);

/*******************************************************************
* create a typed packet reader which will read the packets of the
* appropriate type. Using a typed reader selects only LDP packets, and
* makes sure they are the right type (checksum vs no checksum).
* Not all that necessary now that we know all LDPs have checksums.
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
SwiftHead3* head3 = NULL;

try {
    head.read(r);
    head3 = head.head3();
} catch (Interpreter::Exception& e) {
    /***********************************************************************
    * well, we failed reading the LDP header, which probably means there's
    * no page1. But if the LDP is old enough, we will just go ahead
    * and use the secondary header time to bin the LDP
    ***********************************************************************/
    if(ldp->time() >= ui->getReal("mature_data_time") ) {
        cout << "unknown"<<endl;
        exit(0);
    }
}


/********************************************************
* check if the ID is set to the "unknown" value
* 0xffffffff If so we will use a timeline to generate
* the file name instead of the tertiary header
*******************************************************/
bool id_is_unknown = head3 == NULL ||
                     (head3->target() == 0xffffff && head3->obs() == 0xff );

/***********************************************************
* get the APID so we can determine if this is a BAT packet *
***********************************************************/
bool is_bat = ldp->apid() >= ui->getInteger("first_bat_apid") &&
              ldp->apid() <= ui->getInteger( "last_bat_apid");
              
/*****************************************
* figure out how we will bin the packets *
*****************************************/
PacketBinner* binner = NULL;

if(is_bat) {
    /**********************************************
    * BAT LDPs are all binned by time and they
    * have their own special timeline
    *********************************************/
    TimeLine* timeline = new TimeLine();
    timeline->read(ui->getIstream("bat_timeline"));
    binner = new TimePacketBinner(timeline);

} else if(id_is_unknown) {
    /*********************************************************
    * use the time this probably doesn't happen for NFI LDPs *
    *********************************************************/
    TimeLine* timeline = new TimeLine();
    timeline->read(ui->getIstream("timeline"));
    binner = new TimePacketBinner(timeline);

} else {
    /*************
    * use the ID *
    *************/
    binner = new Head3PacketBinner();
}

/***************************************************
* set the parts of the filename so that they are
* appropriate for LDPs
**************************************************/
binner->dir(ui->getString("dir"));
binner->base("swift_ldp_");

int product_and_index = (ui->getInteger("index") << 16) + ldp->product();

ostringstream tmp;
tmp << "_"<< product_and_index <<".ccsds";
#ifdef FAKING_STRINGSTREAM
tmp << ends;
#endif
binner->ext(tmp.str());

/************************
* now get the file name *
************************/
File* file = NULL;

if(id_is_unknown || is_bat) {
    /**********
    * by time *
    **********/
    double time=0.0;
    
    if(head3==NULL) {
//cerr << "no LDP header"<<endl;
        /**************************************************************
        * We couldn't read the LDP header, so
        * take whatever time we can get. We can only get
        * here if the LDP is mature enough that we can be confident
        * that we won't be getting any more packets for it
        **************************************************************/
        if(ldp->page() != LDPBuffer::unknown ) {
            time = ldp->time();
        } else if(ldp->prev_page() != LDPBuffer::unknown ) {
            time = ldp->prev_time();
        } else {
            /*************************
            * the LDP file was empty *
            *************************/
            cerr << "No packets in the LDP"<<endl;
            exit(1);
        }
        
    } else if(head3->sc_time()->seconds() == 0 ||
              head3->sc_time()->seconds() == 0xffffffff ) {
        /*******************************************************
        * We got the LDP header, but it doesn't have a valid 
        * time in it. So we will get the secondary header time,
        *******************************************************/
//cerr << "invalid time in LDP header"<<endl;
        if(ldp->page() == 1) {
            /*****************************************************
            * the LDP buffer didn't get to page 2 yet, so 
            * we will try pulling the next packet out ourselves
            *****************************************************/
//cerr << "have to read next packet\n";
            CCSDSPacket* generic_p = NULL;
            reader.ldp() >> generic_p;
            LDPPacket* p = dynamic_cast<LDPPacket*>(generic_p);
            
            if(p == NULL) {
                cout << "unknown"<<endl;
                exit(0);
            } 
//cerr << "page="<<p->page()<<endl;

            if(p->page() == 2 || p->time() < ui->getReal("mature_data_time") ) {
                /******************
                * we found page 2 *
                ******************/
                time = p->time();
//cerr << "set time="<<time<<endl;
            } else {
                /********************************************************
                * the LDP doesn't have a page2, and it's fresh enough
                * that it's worth waiting for more pages to arrive
                ********************************************************/
                cout << "unknown"<<endl;
                exit(0);
            }
        } else if(ldp->page() == 2 ||
                  (ldp->page() != LDPBuffer::unknown &&
                   ldp->time() < ui->getReal("mature_data_time")) ) {
            /***************************************
            * Either we have page2, or the LDP is old enough we don't
            * care what page we have
            ************************************************************/
            time = ldp->time();
        } else {
            cout << "unknown"<<endl;
            exit(0);
        }

    } else {
        /*******************************
        * the LDP header time is valid *
        *******************************/
        time = head3->sc_time()->value();
//cerr << "time from LDP header="<<time<<endl;
    }

//cerr << "binning with time "<<setprecision(14)<<time<<endl;
    /*********************************************************
    * now get the file name based on the best available time *
    *********************************************************/
    TimePacketBinner* time_binner = dynamic_cast<TimePacketBinner*>(binner);
    file = time_binner->getFileFromTime(time, ldp->apid());

} else {
    /********
    * by ID *
    ********/
//cerr << "using head3"<<endl;
    Head3PacketBinner* head3_binner = dynamic_cast<Head3PacketBinner*>(binner);
    file = head3_binner->getFileFromHead3(head3, ldp->apid());
}


ostream* out = ui->getOstream("outfile");

//if(file==NULL) cerr << "file is null"<<endl;

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
