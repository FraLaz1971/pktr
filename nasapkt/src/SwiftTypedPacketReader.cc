#include "SwiftTypedPacketReader.h"


#include "PacketTee.h"

/**************************************************************************
* constructor. The "first argument is the name of an APIDentifier
* configuration file, and the second argument is the istream from
* which to read packets.
**************************************************************************/
SwiftTypedPacketReader::SwiftTypedPacketReader(const string& config, 
                                               istream* in)
                       : APIDentifier(config)  {

/*****************************
* initialize the data reader *
*****************************/
r = new Reader(in);

/**************************
* create the output pipes *
**************************/
head2_out   = new Drain(this);
uvot_out    = new Drain(this);
head3_out   = new Drain(this);
ldp_out     = new Drain(this);
acs_out     = new Drain(this);
tdrss_out   = new Drain(this);
generic_out = new Drain(this);
trash_out   = new Drain(this);

/**********************************
* create the CCSDS primary header *
**********************************/
head = new CCSDSHead();

/*******************************
* create the prototype packets *
*******************************/
generic_p  = new CCSDSPacket();
head2_p    = new SwiftPacket();
head3_p    = new SwiftHead3Packet();
ldp_p      = new LDPPacket();
checksum_p = new ChecksumLDPPacket();


} // end of constructor

/**************************************************************************
* destructor
**************************************************************************/
SwiftTypedPacketReader::~SwiftTypedPacketReader() {

delete r; r=NULL;

delete head2_out;     head2_out=NULL;
delete uvot_out;       uvot_out=NULL;
delete head3_out;     head3_out=NULL;
delete ldp_out;         ldp_out=NULL;
delete acs_out;         acs_out=NULL;
delete tdrss_out;     tdrss_out=NULL;
delete generic_out; generic_out=NULL;
delete trash_out;     trash_out=NULL;

delete head; head=NULL;

delete generic_p;   generic_p=NULL;
delete head2_p;       head2_p=NULL;
delete ldp_p;           ldp_p=NULL;
delete checksum_p; checksum_p=NULL;

} // end of constructor

/**************************************************************************
* returns a pointer to the packet object corresponding to a given type
**************************************************************************/
CCSDSPacket* SwiftTypedPacketReader::getPacket(int type) {

    if(      type == HEAD2 )   return head2_p;
    else if( type == UVOT)     return head2_p;
    else if( type == HEAD3 )   return head3_p;
    else if( type == LDP )     return ldp_p;
    else if( type == CHECKSUM) return checksum_p;
    else if( type == ACS)      return head2_p;
    else if( type == TDRSS)    return head2_p;
    else if( type == TRASH)    return generic_p;
    else                       return generic_p;

} // end of getPacket method

/**************************************************************************
* returns a pointer to the drain corresponding to a given type
**************************************************************************/
SwiftTypedPacketReader::Drain* SwiftTypedPacketReader::getDrain(int type) {

    if(      type == HEAD2 )   return head2_out;
    else if( type == UVOT)     return uvot_out;
    else if( type == HEAD3 )   return head3_out;
    else if( type == LDP )     return ldp_out;
    else if( type == CHECKSUM) return ldp_out;
    else if( type == ACS)      return acs_out;
    else if( type == TDRSS)    return tdrss_out;
    else if( type == TRASH)    return trash_out;
    else                       return generic_out;

} // end of getPacket method


/**************************************************************************
* Read a single packet from the istream and send it down the 
* appropriate drain
**************************************************************************/
void SwiftTypedPacketReader::readPacket() throw(Interpreter::Exception) {

//cout << "SwiftTypedPacketReader::readPacket start\n";

/***************************
* read the primary header *
**************************/
head->read(r);

/******************************************
* determine the packet type from the APID *
******************************************/
int type = this->type(head->apid());

/*
  cout << "read primary header type="<<type
       <<" length="<<head->length()
       <<" apid="<<head->apid()
       <<endl;
*/



/***************************************************
* get the packet prototype and make sure there is
* room for all the headers
***************************************************/
CCSDSPacket* p = getPacket(type);
SwiftTypedPacketReader::Drain* drain = getDrain(type);


if( type != TRASH && (
    (p!= generic_p && !head->hasHead2() ) ||
     head->version() != 0 ||
     head->type()    != 0 ||
   (type!= UVOT && (head->isFirst()==0 || head->isLast()==0) ) ||
   (int)(head->length()) < p->minimumLength()                     )) {
    /******************************************
    * messed up packet, so revert to
    * a generic packet.
    * We will now send these to the trash instead of
    * to the generic output. We have been seeing alot of garbage
    * packets in the ground test data and we'd be better off 
    * ignoring them than mixing them with the unidentified APIDS.
    * At this stage in the mission we don't need to be vigilant for 
    * flight software bugs which will corrupt the packets.
    * -ED 2004-08-05
    ******************************************/
/*
    cout << "    packet messed up"
         << " version="<<head->version()
         << " type="<<head->type()
         << " isFirst="<<head->isFirst()
         << " isLast="<<head->isLast()
   //      << " p="<<p<<" generic="<<generic_p
         << " length="<<head->length()<<" min_mength="<<p->minimumLength()
         <<endl;
*/
    p = generic_p;
    drain=trash_out;
}



/***************************************************************
* read the rest of the packet and send it down the right drain *
***************************************************************/
p->read(*head,r);
*drain << p;

} // end of readPacket method


/**************************************************************************
* Read all available packets from the istream.
**************************************************************************/
void SwiftTypedPacketReader::pump() {


try {
    /************************************************
    * loop over packets until we catch an exception *
    ************************************************/
    for(;;) readPacket();

} catch(const Interpreter::Exception &e) { 
    /*****************************************
    * had an error reading - probably EOF -
    * so send a NULL "end of stream" packet
    * pointer down all the outlet pipes
    *****************************************/
    *head2_out   << (CCSDSPacket*)NULL;
    *uvot_out    << (CCSDSPacket*)NULL;
    *head3_out   << (CCSDSPacket*)NULL;
    *ldp_out     << (CCSDSPacket*)NULL;
    *acs_out     << (CCSDSPacket*)NULL;
    *tdrss_out   << (CCSDSPacket*)NULL;
    *trash_out   << (CCSDSPacket*)NULL;
    *generic_out << (CCSDSPacket*)NULL;

} // end of catch phrase


} // end of pump method



/*****************************************************************************
* constructor for the Drain inner class
*****************************************************************************/
SwiftTypedPacketReader::Drain::Drain(SwiftTypedPacketReader* reader) {
    this->reader = reader;
    waiting=false;
    packet=NULL;
}

/*****************************************************************************
*
*****************************************************************************/
void SwiftTypedPacketReader::Drain::pushIn(CCSDSPacket* p) {
    if(waiting) {
        waiting=false;
        packet = p;
    } else {
        if(sink != NULL) *sink << p;
    }
} // end of pushIn method

/*****************************************************************************
*
*****************************************************************************/
CCSDSPacket* SwiftTypedPacketReader::Drain::pullOut() {

    startWaiting();
    try {
        while(isWaiting()) {
            reader->readPacket();
        }

        return stopWaiting();

    } catch(Interpreter::Exception& e) {
        stopWaiting();
        return (CCSDSPacket*)NULL;
    }
} // end of pull out method

/*****************************************************************************
*
*****************************************************************************/
void SwiftTypedPacketReader::Drain::connectToSink(  PacketPipe& sink  ) {

    /***************************
    * inherit superclass stuff *
    ***************************/
    PacketPipe::connectToSink(sink);
    
    /********************************
    * see if we got hooked to a tee *
    ********************************/
    PacketTee* tee = dynamic_cast<PacketTee*>(&sink);
    if(tee == NULL) return;
    
    /**********************************************************
    * loop over all the other sources connected to this drain *
    **********************************************************/
    for(int i=0; i<tee->getSourceCount(); ++i) {
        PacketPipe* peer = tee->getSource(i);

        /****************
        * skip this one *
        ****************/
        if(peer == this) continue;

        /***********************************************
        * see if it's another drain in the same reader *
        ***********************************************/
        Drain* drain = dynamic_cast<Drain*>(peer);
        if(drain == NULL) continue;
        if(drain->reader != reader) continue;
        
        /*********************************
        * if we get here we have found a
        * fellow drain teed with this one
        **********************************/
        peers.insert(peers.end(), drain);
        drain->peers.insert(drain->peers.end(), this);


    } // end of loop over fellow sources to the tee
    

} // end of connectToSink method

/****************************************************************************
* Mark this drain and all its peers as waiting
****************************************************************************/
void SwiftTypedPacketReader::Drain::startWaiting() {

    waiting = true;

    for(set<Drain*>::iterator it = peers.begin();
        it != peers.end();
        ++it ) {
        Drain* drain = *(it);

        drain->waiting=true;
    }
} // end of startWaiting method

/****************************************************************************
* check if this drain and all of its peers are waiting
****************************************************************************/
bool SwiftTypedPacketReader::Drain::isWaiting() {

    if(!waiting) return false;

    for(set<Drain*>::iterator it = peers.begin();
        it != peers.end();
        ++it ) {
        Drain* drain = *(it);

        if(!drain->waiting) return false;
    }

    return true;

} // end of isWaiting method

/****************************************************************************
*
****************************************************************************/
CCSDSPacket* SwiftTypedPacketReader::Drain::stopWaiting() {

    CCSDSPacket* p =NULL;

    if(!waiting) p = packet;
    else        waiting=false;

    for(set<Drain*>::iterator it = peers.begin();
        it != peers.end();
        ++it ) {
        Drain* drain = *(it);



        if(!drain->waiting) p = drain->packet;
        else                    drain->waiting=false;

    }

    return p;

} // end of startWaiting method
