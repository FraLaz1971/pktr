#ifndef SWIFT_IDCOUNTER_H
#define SWIFT_IDCOUNTER_H

#include "PacketPipe.h"

class SwiftIDCounter : public PacketPipe {

private:
    /*******
    * data *
    *******/
    bool first;
    unsigned char last_obs;
    unsigned int last_target; 

public:
    /**************
    * constructor *
    **************/
    SwiftIDCounter();

    /*************
    * destructor *
    *************/
    virtual ~SwiftIDCounter() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    CCSDSPacket* operateOnPacket(CCSDSPacket* generic_p);

private:
    void report(unsigned char obs, unsigned int target);

}; // end of SwiftIDCounter class

#endif // SWIFT_IDCOUNTER_H
