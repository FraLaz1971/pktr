#ifndef PACKET_COUNTER_H
#define PACKET_COUNTER_H

#include "PacketPipe.h"
#include <map>

/*****************************************************************************
* This object counts the packets flowing throw it by APID.
*****************************************************************************/
class PacketCounter : public PacketPipe {

private:
    /*******
    * data *
    *******/
    map<int,int> count;
    int total;


public:
    /**************
    * constructor *
    **************/
    PacketCounter();

    /*************
    * destructor *
    *************/
    virtual ~PacketCounter() {}

    /************
    * accessors *
    ************/
    virtual int getCount(int apid);
    virtual int getCount();

    /**********
    * methods *
    **********/
    virtual void reset();
    virtual CCSDSPacket* operateOnPacket(CCSDSPacket* p);

    virtual void report(ostream* out);

}; // end of PacketCounter class

#endif // PACKET_COUNTER_H
