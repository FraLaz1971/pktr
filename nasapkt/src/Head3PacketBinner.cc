#include "Head3PacketBinner.h"

/****************************************************************************
* constructor
****************************************************************************/
Head3PacketBinner::Head3PacketBinner() {

time_scale = 86400;
_base = "swift_head3_";
_ext = ".ccsds";

} // end of constructor


/****************************************************************************
* return the file for a given teritary header, based on the APID, target/obs ID
* and time.
****************************************************************************/
File* Head3PacketBinner::getFileFromHead3(SwiftHead3* head3, int apid) {


int time = (int)(head3->sc_time()->value())/time_scale;

unsigned int obsID = (head3->target() << 8) + head3->obs();

return getFileNearTime(apid, time, obsID );


} // end of getFile method


/****************************************************************************
* return the file for a particular packet based on the APID, target/obs ID
* and time.
****************************************************************************/
File* Head3PacketBinner::getFile(CCSDSPacket* generic_p) {


SwiftHead3Packet* p = dynamic_cast<SwiftHead3Packet*>(generic_p);
if(p==NULL) return NULL;

return getFileFromHead3(p->head3(), p->apid() );


} // end of getFile method
