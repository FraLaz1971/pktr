#include "Head3TimePacketBinner.h"

#include "SwiftHead3Packet.h"

/*********************************************************************
* constructor
*********************************************************************/
Head3TimePacketBinner::Head3TimePacketBinner(TimeLine* timeline) : TimePacketBinner(timeline) {
} // end of constructor


/****************************************************************************
* return the file for a particular packet
****************************************************************************/
File* Head3TimePacketBinner::getFile(CCSDSPacket* generic_p) {

    /************************************
    * cast the packet as a head2 packet *
    ************************************/
    SwiftHead3Packet* p = dynamic_cast<SwiftHead3Packet*>(generic_p);
    if(p==NULL) return NULL;

    /***********************************
    * look up the time in the timeline *
    ***********************************/
    return getFileFromTime(p->sc_time(), p->apid());


} // end of getFile method
