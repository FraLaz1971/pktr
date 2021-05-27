#ifndef FLOW_PROBE_H
#define FLOW_PROBE_H

#include "PacketPipe.h"

class FlowProbe : public PacketPipe {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    FlowProbe();

    /*************
    * destructor *
    *************/
    virtual ~FlowProbe() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual CCSDSPacket* operateOnPacket(CCSDSPacket* p);

}; // end of FlowProbe class

#endif // FLOW_PROBE_H
