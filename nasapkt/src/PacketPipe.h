#ifndef PACKET_PIPE_H
#define PACKET_PIPE_H

#include "CCSDSPacket.h"

/******************************************************************************
* This is a base class for a whole range of classes for processing CCSDSPacket
* objects. The idea is to construct a number of modules, each doing a different
* thing to the packets and to hook them together like plumbing.
* 
* This class does nothing to the packets, but provides the functionality
* for connecting modules together and for pushing packets into or pulling
* them out of a chain of pipes.
*
* This class overrides a number of operators to provide convenient syntax.
* Typical usage is to create a number of PacketPipe (or some subclass)
* objects and then connect them together like this "pipe1 | pipe2 | pipe3".
* Then if you have a pointer to a CCSDS packet "p", you may push it into
* the pipe chain like this: "pipe1 << p". Such a packet will first be
* operated on by pipe1, then by pipe2, etc.
* Alternately, you may pull a packet out of a pipe like this: "pipe3 >> p".
* Most types of pipes support both push in and pull out, though sometimes
* one is more natuaral or effient than the other.
*
* Packets are passed between pipes by reference, and a NULL pointer is used
* to signal the end of the stream of packets.
* 
******************************************************************************/
class PacketPipe {

public:
    /*************
    * class data *
    *************/
    static const int ALL_UNIQUE=1;

protected:
    /*******
    * data *
    *******/
    PacketPipe* source;
    PacketPipe* sink;

public:
    /**************
    * constructor *
    **************/
    PacketPipe();

    /************
    * accessors *
    ************/
    virtual int hints();
    virtual void hints(int);

    /**********
    * methods *
    **********/
    virtual void connectToSource(PacketPipe& source);
    virtual void connectToSink(  PacketPipe& sink  );

    virtual void disconnectFromSource();
    virtual void disconnectFromSink();

    virtual void pushIn(CCSDSPacket* p);
    virtual CCSDSPacket* pullOut();

    virtual void pump();

    /***********************
    * overloaded operators *
    ***********************/
    friend PacketPipe& operator|(PacketPipe& source, PacketPipe& sink);

    friend PacketPipe& operator>>(PacketPipe& source, CCSDSPacket* &p);
    friend PacketPipe& operator<<(PacketPipe& sink, CCSDSPacket* p);

protected: 
    /******************************************************************
    * override this method to filter the packets as they come through 
    * the pipe.
    ******************************************************************/
    virtual CCSDSPacket* operateOnPacket(CCSDSPacket* p) {return p;}


}; // end of PacketPipe class

#endif // PACKET_PIPE_H
