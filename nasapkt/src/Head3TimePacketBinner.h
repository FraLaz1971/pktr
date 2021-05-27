#ifndef HEAD3TIME_PACKET_BINNER_H
#define HEAD3TIME_PACKET_BINNER_H

#include "TimePacketBinner.h"

class Head3TimePacketBinner : public TimePacketBinner {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    Head3TimePacketBinner(TimeLine* timeline);

    /*************
    * destructor *
    *************/
    virtual ~Head3TimePacketBinner() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
 protected:
    /******************************
    * method for routing a packet *
    ******************************/
    virtual File* getFile(CCSDSPacket* generic_p);


}; // end of Head3TimePacketBinner class

#endif // HEAD3TIME_PACKET_BINNER_H
