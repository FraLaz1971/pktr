#ifndef TIME_PACKET_BINNER_H
#define TIME_PACKET_BINNER_H

#include "PacketBinner.h"
#include "SwiftPacket.h"
#include "TimeLine.h"

/*****************************************************************************
* This class bins files by observation by looking up the Swift
* secondary header time
* in a TimeLine object to determine the target/obs ID.
******************************************************************************/
class TimePacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    int time_scale;
    TimeLine* timeline;
    
public:
    /**************
    * constructor *
    **************/
    TimePacketBinner(TimeLine* timeline);
    
public:
    virtual File* getFileFromTime(double time, int apid);
protected:
    /******************************
    * method for routing a packet *
    ******************************/
    virtual File* getFile(CCSDSPacket* generic_p);



}; // end of TimePacketBinner class


#endif //  TIME_PACKET_BINNER_H
