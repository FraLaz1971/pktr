#ifndef PACKET_DELETER_H
#define PACKET_DELETER_H

#include "PacketPipe.h"
#include "CCSDSPacket.h"

/*****************************************************************************
* This pipe deletes every packet which passes through it, freeing the memory
* allocated to the packet. You can pull packets out of it, but it will always
* give NULL (end of stream). Likewise for every packet pushed into it it sends
* NULL out the end.
* 
* You have to be careful when using this class. Most packet streams
* reuse the same packet over and over again, so deleting that packet would
* be bad. On the other hand if you have a stream of unique packets, such as
* might come out of a CCSDS merger or a packet tank, then you really ought to 
* use a PacketDeleter at the end of your chain, or those packets could
* fill up the memory.
*****************************************************************************/
class PacketDeleter : public PacketPipe {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    PacketDeleter();

    /*************
    * destructor *
    *************/
    virtual ~PacketDeleter() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
protected:
    virtual CCSDSPacket* operateOnPacket(CCSDSPacket* p);

}; // end of PacketDeleter class

#endif // PACKET_DELETER_H
