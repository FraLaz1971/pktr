#ifndef HEAD3_PACKET_BINNER_H
#define HEAD3_PACKET_BINNER_H

#include "PacketBinner.h"
#include "SwiftHead3Packet.h"

/****************************************************************************
* This class bins packets according to the target/obs ID in its 
* Swift standard tertiary header. The IDs are time-uniqued. 
* See the superclass for more information.
****************************************************************************/ 
class Head3PacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    int time_scale;


public:
    /**************
    * constructor *
    **************/
    Head3PacketBinner();

    virtual File* getFileFromHead3(SwiftHead3* head3, int apid);

protected:
    /******************************
    * method for routing a packet *
    ******************************/
    virtual File* getFile(CCSDSPacket* generic_p);



}; // end of Head3PacketBinner class


#endif //  HEAD3_PACKET_BINNER_H
