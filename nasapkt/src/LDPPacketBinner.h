#ifndef LDP_PACKET_BINNER_H
#define LDP_PACKET_BINNER_H

#include "PacketBinner.h"
#include "LDPPacket.h"

/****************************************************************************
* This class bins Swift Large Data Product packets by APID, product number
* and time. In other words it produces one file per large data product.
* See the superclass for more information.
****************************************************************************/
class LDPPacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    int time_scale;

public:
    /**************
    * constructor *
    **************/
    LDPPacketBinner(int time_scale);

    /**************
    * destructor *
    **************/
    virtual ~LDPPacketBinner() { }


protected:
    /******************************
    * method for routing a packet *
    ******************************/
    virtual File* getFile(CCSDSPacket* generic_p);
    virtual File* getFile(int apid, int time, unsigned int id);
    virtual File* getFileNearTime(int apid, int time, unsigned int id);



}; // end of LDPPacketBinner class

#endif //  LDP_PACKET_BINNER_H
