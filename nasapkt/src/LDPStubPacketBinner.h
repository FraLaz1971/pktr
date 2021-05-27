#ifndef LDPSTUB_PACKET_BINNER_H
#define LDPSTUB_PACKET_BINNER_H

#include "PacketBinner.h"
#include "LDPStubRepository.h"

class LDPStubPacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    map<int, LDPStubRepository*> repositories;


public:
    /**************
    * constructor *
    **************/
    LDPStubPacketBinner();

    /*************
    * destructor *
    *************/
    virtual ~LDPStubPacketBinner() {}

    /************
    * accessors *
    ************/
    virtual LDPStubRepository* getRepository(int apid);
    

    /**********
    * methods *
    **********/
    virtual File* getFile(CCSDSPacket* generic_p);
    virtual void initRepositories();

}; // end of LDPStubPacketBinner class

#endif // LDPSTUB_PACKET_BINNER_H
