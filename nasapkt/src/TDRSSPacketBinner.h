#ifndef TDRSS_PACKET_BINNER_H
#define TDRSS_PACKET_BINNER_H

#include "PacketBinner.h"
#include "SwiftPacket.h"

/****************************************************************************
* This class bins packets according to the target/obs ID in its 
* Swift standard tertiary header. The IDs are time-uniqued. 
* See the superclass for more information.
****************************************************************************/ 
class TDRSSPacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    int time_scale;


public:
    /**************
    * constructor *
    **************/
    TDRSSPacketBinner();

protected:
    /******************************
    * method for routing a packet *
    ******************************/
    virtual File* getFile(CCSDSPacket* generic_p);



}; // end of TDRSSPacketBinner class


#endif //  TDRSS_PACKET_BINNER_H
