#ifndef PACKET_READER_H
#define PACKET_READER_H

#include "CCSDSPacket.h"
#include "PacketPipe.h"
#include "Reader.h"

/****************************************************************************
* This class reads packets from a given istream.
* Note that it will only read packets of a single type specified in the
* constructor. 
*
* When you pull a packet out of a PacketReader, it reads a new packet from
* the istream. If you push a packet into a PacketReader it will come out
* the other side like a straight pipe. This might be useful in some cases
* but isn't the sort of thing you want in general.
*
* See SwiftTypedPacketReader for a more sophisticated reader which can
* produce different types of packets depending on APID.
****************************************************************************/
class PacketReader : public PacketPipe {


private:
    /*******
    * data *
    *******/
    Reader* r;
    CCSDSPacket* p;

public:
    /**************
    * constructor *
    **************/
    PacketReader(istream* in, CCSDSPacket* p);

    /*************
    * destructor *
    *************/
    virtual ~PacketReader() { }

    /**********
    * methods *
    **********/
  //  virtual void connectToSource(PacketPipe& source) { }
  //  virtual void pushIn(CCSDSPacket* p) {}

    virtual CCSDSPacket* pullOut();
    virtual void pump();

}; // end of PacketReader class


#endif //  PACKET_READER_H
