#ifndef PACKET_WRITER_H
#define PACKET_WRITER_H

#include "CCSDSPacket.h"
#include "PacketPipe.h"
#include "Writer.h"

/******************************************************************************
* This class writes packets to an ostream. If you pull a packet out of a
* PacketWriter, it will simply act as a straight pipe, which is probably not
* what you want.
******************************************************************************/
class PacketWriter : public PacketPipe {


private:
    /*******
    * data *
    *******/
    Writer* w;

public:
    /**************
    * constructor *
    **************/
    PacketWriter(Writer* w) {
        this->w = w;
    }

    PacketWriter(ostream* out) {

        w = new Writer(out);

    } // end of constructor

    /************
    * accessors *
    ************/
    virtual Writer* getWriter() { return w; }



    /**********
    * methods *
    **********/
protected:
    virtual CCSDSPacket* operateOnPacket(CCSDSPacket* p);


}; // end of PacketWriter class


#endif //  PACKET_WRITER_H
