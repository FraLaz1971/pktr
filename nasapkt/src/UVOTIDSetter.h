#ifndef UVOTIDSETTER_H
#define UVOTIDSETTER_H

#include "SwiftIDSetter.h"

class UVOTIDSetter : public SwiftIDSetter {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    UVOTIDSetter();

    /*************
    * destructor *
    *************/
    virtual ~UVOTIDSetter() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    CCSDSPacket* operateOnPacket(CCSDSPacket* generic_p);


}; // end of UVOTIDSetter class

#endif // UVOTIDSETTER_H
