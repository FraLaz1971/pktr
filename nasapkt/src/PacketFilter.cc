#include "PacketFilter.h"

/*********************************************************************
* constructor
*********************************************************************/
PacketFilter::PacketFilter() {
} // end of constructor

/**************************************************************************
*
**************************************************************************/
void PacketFilter::pushIn(CCSDSPacket* p) {

    if(! accept(p) ) return;

    if(sink!=NULL) sink->pushIn(p);;

}

/**************************************************************************
*
**************************************************************************/
CCSDSPacket* PacketFilter::pullOut() {

    if(source==NULL) return NULL;

    CCSDSPacket* p = NULL;
    while(true) {
        p = source->pullOut();
        if(p==NULL) return NULL;
        if(accept(p) ) return p;
    }

    return NULL;

} // end of pullOut method


/****************************************************************************
* pull packets from the source 
* and deliver them to the sink until no packets are left. This method is
* useful, e.g. if you have a PacketReader at one end of a chain and
* a PacketWriter at the other end. Then you can call the pump() method
* on some intermediate pipe to process all the packets in some input file.
****************************************************************************/
void PacketFilter::pump() {

    CCSDSPacket* p=NULL;

    /********************************************
    * can't pump anything if there is no source *
    ********************************************/
    if(source==NULL ) {
        *sink << p;
        return;
    }

    /********************************************
    * loop over the packets until we get a null *
    ********************************************/
    do  {

        *source >> p;
        p = operateOnPacket(p);
        if(! accept(p) ) continue;
        if(sink) *sink << p;

    } while(p!=NULL);

    

} // end of pump method

