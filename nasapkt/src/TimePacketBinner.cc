#include "TimePacketBinner.h"

/****************************************************************************
* constructor - note you must specify the TimeLine to be used to look up
* target/obs IDs.
****************************************************************************/
TimePacketBinner::TimePacketBinner(TimeLine* timeline) {

    this->timeline = timeline;

    time_scale = 86400;
    _base = "swift_head2_";
    _ext = ".ccsds";

} // end of constructor


/****************************************************************************
* return the file for a particular packet
****************************************************************************/
File* TimePacketBinner::getFileFromTime(double time, int apid) {


    TimeInterval* interval = timeline->findInterval(time);

    if(interval == NULL) return NULL;
    unsigned int obsID = interval->id();

    /****************************
    * calculate the scaled time *
    ****************************/
    int high_time = (int)(time)/time_scale;

    //cout << "TimePacketBinner::getFile time="<<high_time<<" id="<<obsID<<"\n";

    return getFileNearTime(apid, high_time, obsID );


} // end of getFile method

/****************************************************************************
* return the file for a particular packet
****************************************************************************/
File* TimePacketBinner::getFile(CCSDSPacket* generic_p) {

    /************************************
    * cast the packet as a head2 packet *
    ************************************/
    SwiftPacket* p = dynamic_cast<SwiftPacket*>(generic_p);
    if(p==NULL) return NULL;

    /***********************************
    * look up the time in the timeline *
    ***********************************/
    return getFileFromTime(p->time(), p->apid());


} // end of getFile method
