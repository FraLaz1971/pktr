#ifndef ACSPACKET_BINNER_H
#define ACSPACKET_BINNER_H

#include "PacketBinner.h"
#include "ACS.h"

class ACSPacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    int time_scale;
    ACS acs;



public:
    /**************
    * constructor *
    **************/
    ACSPacketBinner();

    /*************
    * destructor *
    *************/
    virtual ~ACSPacketBinner() {}

protected:
    /******************************
    * method for routing a packet *
    ******************************/
    virtual File* getFile(CCSDSPacket* generic_p);

}; // end of ACSPacketBinner class

#endif // ACSPACKET_BINNER_H
