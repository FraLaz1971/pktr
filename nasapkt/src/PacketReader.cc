#include "PacketReader.h"

/***************************************************************************
* constructor. You must specify an istream and a prototype packet cast
* as a generic CCSDSPacket. The reader will use that packet to read from 
* the istream.
***************************************************************************/
PacketReader::PacketReader(istream* in, CCSDSPacket* p) {

    r = new Reader(in);
    this->p = p;

} // end of constructor



/**************************************************************************
* read a packet from the underlying istream.
**************************************************************************/
CCSDSPacket* PacketReader::pullOut() {


try { p->read(r); }
catch(const Interpreter::Exception &e) { return 0; }

/*
cout << "reading apid="<<p->apid()
     <<" size="<<p->size()<<" length="<<p->length()<<"\n";

*/


return p;

} // end of pullOut method


/**************************************************************************
* read all the packets and send them down the pipe
**************************************************************************/
void PacketReader::pump() {

if(sink==NULL) return;

try {
    /***********************
    * read all the packets *
    ***********************/
    while(true) {
        p->read(r);
        *sink << p;
    }

} catch(const Interpreter::Exception &e) { 
    /***************************
    * probably encountered EOF *
    ***************************/
    *sink << NULL; 
}



} // end of pump method

