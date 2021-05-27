#include "TDRSSPacketBinner.h"

/****************************************************************************
* constructor
****************************************************************************/
TDRSSPacketBinner::TDRSSPacketBinner() {

time_scale = 86400;
_base = "swift_head2_";
_ext = ".ccsds";

} // end of constructor


/****************************************************************************
* return the file for a particular packet based on the APID, target/obs ID
* and time.
****************************************************************************/
File* TDRSSPacketBinner::getFile(CCSDSPacket* generic_p) {

    /***************************************
    * make sure there's a secondary header *
    ***************************************/
    SwiftPacket* p = dynamic_cast<SwiftPacket*>(generic_p);
    if(p==NULL) return NULL;

    /*************************************************
    * read the target number form the first 4 bytes
    * throwing away the highest bytes
    *************************************************/
    Reader* r = p->dataReader();

    unsigned int id = r->readUnsignedInt(4);

    unsigned int obs = (id >> 24) & 0xff;

    id <<= 8;
    id |= obs;

    /**************
    * scaled time *
    **************/
    int time = (int)(p->time())/time_scale;


    return getFileNearTime(p->apid(),time, id );


} // end of getFile method
