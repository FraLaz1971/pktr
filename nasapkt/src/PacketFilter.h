#ifndef PACKET_FILTER_H
#define PACKET_FILTER_H

#include "PacketPipe.h"

class PacketFilter : public PacketPipe {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    PacketFilter();

    /*************
    * destructor *
    *************/
    virtual ~PacketFilter() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual bool accept(CCSDSPacket* p) =0;

    virtual void pushIn(CCSDSPacket* p);
    virtual CCSDSPacket* pullOut();

    virtual void pump();


}; // end of PacketFilter class

#endif // PACKET_FILTER_H
