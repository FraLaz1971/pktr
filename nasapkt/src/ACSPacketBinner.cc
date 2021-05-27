#include "ACSPacketBinner.h"

#include "SwiftPacket.h"

/*********************************************************************
* constructor
*********************************************************************/
ACSPacketBinner::ACSPacketBinner() {

    time_scale = 86400;
    _base = "swift_acs_";
    _ext = ".ccsds";

} // end of constructor

/****************************************************************************
* return the file for a particular packet based on the APID, target/obs ID
* and time.
****************************************************************************/
File* ACSPacketBinner::getFile(CCSDSPacket* generic_p) {

    SwiftPacket* p = dynamic_cast<SwiftPacket*>(generic_p);
    if(p==NULL) return NULL;

    /*******************************
    * read the ACS packet contents *
    *******************************/
    Reader* r = p->dataReader();
    acs.read(r);

    istream* in = r->getStream();
    streambuf* buf =in->rdbuf();  
    delete buf;
    delete in;
    delete r;

    /***********************************
    * construct the scaled time and ID *
    ***********************************/
    int time = (int)(acs.time()->value())/time_scale;
    unsigned int obsID = (acs.target() << 8) + acs.obs();

    return getFileNearTime(p->apid(), time, obsID );



} // end of getFile method
