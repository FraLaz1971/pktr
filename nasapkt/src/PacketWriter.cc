#include "PacketWriter.h"



/**************************************************************************
* write a packet to the underlying ostream.
**************************************************************************/
CCSDSPacket* PacketWriter::operateOnPacket(CCSDSPacket* p) {

    if(p!=NULL) {

        try {  p->write(w); } 
        catch(const Interpreter::Exception &e) { return NULL; }

    }

    return p;

} // end of operateOnPacket method


