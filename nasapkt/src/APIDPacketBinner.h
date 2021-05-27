#ifndef APID_PACKET_BINNER_H
#define APID_PACKET_BINNER_H

#include "PacketBinner.h"
#include <string>

/****************************************************************************
* This class bins packets according to the target/obs ID in its 
* Swift standard tertiary header. The IDs are time-uniqued. 
* See the superclass for more information.
****************************************************************************/ 
class APIDPacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    const char* format;
    char* filename;

public:
    /**************
    * constructor *
    **************/
    APIDPacketBinner(const string& dir);
    
    /*************
    * destructor *
    *************/
    ~APIDPacketBinner();

protected:
    /******************************
    * method for routing a packet *
    ******************************/
    virtual File* getFile(CCSDSPacket* generic_p);



}; // end of APIDPacketBinner class


#endif //  APID_PACKET_BINNER_H
