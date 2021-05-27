#ifndef SWIFT_IDSETTER_H
#define SWIFT_IDSETTER_H

#include "PacketPipe.h"

class SwiftIDSetter : public PacketPipe {

private:
    /*******
    * data *
    *******/
    unsigned char obs;
    unsigned int target;
    unsigned char target_bytes[3];


public:
    /**************
    * constructor *
    **************/
    SwiftIDSetter();

    /*************
    * destructor *
    *************/
    virtual ~SwiftIDSetter() {}

    /************
    * accessors *
    ************/
    void setObs(unsigned char obs);
    void setTarget(unsigned int target);

    unsigned char getObs();
    unsigned int getTarget();
    

    /**********
    * methods *
    **********/
    CCSDSPacket* operateOnPacket(CCSDSPacket* generic_p);

}; // end of SwiftIDSetter class

#endif // SWIFT_IDSETTER_H
