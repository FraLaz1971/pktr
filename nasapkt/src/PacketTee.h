#ifndef PACKET_TEE_H
#define PACKET_TEE_H

#include "PacketPipe.h"
#include <vector>

/*****************************************************************************
* A normal pipe can only have one upstream and one downstream connection.
* This class allows you to make as many connections as you want using the
* usual PacketPipe "|" operator syntax. New connections do not invalidate
* the ones previously made.
*****************************************************************************/
class PacketTee : public PacketPipe {

protected:
    /*******
    * data *
    *******/
    vector<PacketPipe* > sources;
    vector<PacketPipe* > sinks;

public:
    /*************
    * destructor *
    *************/
    virtual ~PacketTee() {}

protected:
    /************
    * accessors *
    ************/
    virtual int hints();
    virtual void hints(int flags);
public:
    virtual int getSourceCount() { return sources.size(); }
    virtual PacketPipe* getSource(int index) { return sources[index]; }

    /**********
    * methods *
    **********/
protected:
    virtual void connectToSource(PacketPipe& source);
    virtual void connectToSink(PacketPipe& sink );

    virtual void disconnectFromSource();
    virtual void disconnectFromSink();

    virtual void pushIn(CCSDSPacket* p);
    virtual CCSDSPacket* pullOut();

    virtual void pump();


}; // end of PacketTee class


#endif //  PACKET_TEE_H
