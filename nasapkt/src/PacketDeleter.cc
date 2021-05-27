#include "PacketDeleter.h"

/*********************************************************************
* constructor
*********************************************************************/
PacketDeleter::PacketDeleter() {
} // end of constructor

/*********************************************************************
*
*********************************************************************/
CCSDSPacket* PacketDeleter::operateOnPacket(CCSDSPacket* p) {

    if(p != NULL) delete p;
    return NULL;

} 
