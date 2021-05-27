#ifndef SWIFT_TYPED_PACKET_READER_H
#define SWIFT_TYPED_PACKET_READER_H

#include <set>

#include "ChecksumLDPPacket.h"
#include "SwiftHead3Packet.h"
#include "PacketPipe.h"
#include "APIDentifier.h"

/****************************************************************************
* This class reads packets from an istream. The type of each packet is
* determined by examining the packet's APID, and the packets of different
* types are routed to different output PacketPipes.
*
* Typical usage would be to create a SwiftTypedPacketReader object,
* then call the head2(), head3(), uvot(), and ldp() methods to 
* connect the "outlet pipes"  to the desired processing plumbing, and then  
* call the pump() method  to make everything go.
*
* Instead of calling pump(), you may pull packets from any of the outlet
* pipes. This causes packets to be read and distributed to the appropriate
* pipes until a packet comes out of the pipe from which you are pulling.
*
* LDP packets will be of the appropriate type (plain or with checksum)
* but all LDP packets go to the same output drain.
****************************************************************************/
class SwiftTypedPacketReader : public APIDentifier {

/******************************************************************
* This is a simple extension of a plain pipe which can detect
* when a packet passes through it. We need this to implement
* pulling packets out of the various SwiftTypedPacketReader
* drains.
******************************************************************/
class Drain : public PacketPipe {

    private: 
        bool waiting;
        CCSDSPacket* packet;
        set<Drain*> peers;

        SwiftTypedPacketReader* reader;
    public:
        /**************
        * constructor *
        **************/
        Drain(SwiftTypedPacketReader* reader);
        virtual ~Drain() {}


        virtual void pushIn(CCSDSPacket* p);
        virtual CCSDSPacket* pullOut();
        virtual void connectToSink(  PacketPipe& sink  );

    private:
        virtual void startWaiting();
        virtual bool isWaiting();
        virtual CCSDSPacket* stopWaiting();

}; // end of Drain class

private:
    /*******
    * data *
    *******/
    Reader* r;

    Drain*   head2_out;
    Drain*    uvot_out;
    Drain*   head3_out;
    Drain*     ldp_out;
    Drain*     acs_out;
    Drain*   tdrss_out;
    Drain* generic_out;
    Drain*   trash_out;

    CCSDSHead* head;

    CCSDSPacket*        generic_p;
    SwiftPacket*          head2_p;
    SwiftHead3Packet*     head3_p;
    LDPPacket*              ldp_p;
    ChecksumLDPPacket* checksum_p;

public:
    /**************
    * constructor *
    **************/
    SwiftTypedPacketReader(const string& config, istream* in);

    /*************
    * destructor *
    *************/
    virtual ~SwiftTypedPacketReader();

    /************
    * accessors *
    ************/
    virtual PacketPipe&   head2() { return   *head2_out; }
    virtual PacketPipe&   head3() { return   *head3_out; }
    virtual PacketPipe&    uvot() { return    *uvot_out; }
    virtual PacketPipe&     ldp() { return     *ldp_out; }
    virtual PacketPipe&     acs() { return     *acs_out; }
    virtual PacketPipe&   tdrss() { return   *tdrss_out; }
    virtual PacketPipe& generic() { return *generic_out; }
    virtual PacketPipe&   trash() { return   *trash_out; }

protected:
    virtual CCSDSPacket* getPacket(int type);
    virtual       Drain* getDrain( int type);

public:
    /**********
    * methods *
    **********/
    virtual void readPacket() throw(Interpreter::Exception);
    virtual void pump();


};

#endif //  SWIFT_TYPED_PACKET_READER_H
