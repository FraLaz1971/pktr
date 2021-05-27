#include "SwiftHead3Packet.h"

/*****************************************************************************
* construct an empty packet
*****************************************************************************/
SwiftHead3Packet::SwiftHead3Packet() : SwiftPacket() {

_head3 = new SwiftHead3();

} // end of empty packet constructor

/*****************************************************************************
* construct a packet with a given number of data bytes
*****************************************************************************/
SwiftHead3Packet::SwiftHead3Packet(int size) : SwiftPacket(size) {

sizeAccordingToHeader(size);

_head3 = new SwiftHead3();

} // end of packet constructor

/*****************************************************************************
* copy constructor
*****************************************************************************/
SwiftHead3Packet::SwiftHead3Packet(const SwiftHead3Packet& orig) 
                 : SwiftPacket(orig) {

sizeAccordingToHeader(orig.size() );

_head3= new SwiftHead3();

target(orig.target());
obs(orig.obs());
sc_time(orig.sc_time());
utcf_time(orig.utcf_time());


} // end of copy constructor


/*****************************************************************************
* return a pointer to a new copy of this packet cast as a generic CCSDSPacket
* This is basicly a virtual wrapper around the copy constructor
*****************************************************************************/
CCSDSPacket* SwiftHead3Packet::copy() { return new SwiftHead3Packet(*this); }


/*****************************************************************************
* size of the secondary header in bytes
*****************************************************************************/
int SwiftHead3Packet::extraHeadSize() const { 

    return SwiftPacket::extraHeadSize() + SwiftHead3::size; 
}


/*****************************************************************************
* read the secondary header
*****************************************************************************/
void SwiftHead3Packet::readExtraHeaders(Reader* r) 
     throw(Interpreter::Exception) {

    SwiftPacket::readExtraHeaders(r);
    _head3->read(r);

} // end of readExtraHeaders method

/*****************************************************************************
* write the secondary header
*****************************************************************************/
void SwiftHead3Packet::writeExtraHeaders(Writer* w) 
     throw(Interpreter::Exception) {

    SwiftPacket::writeExtraHeaders(w);
    _head3->write(w);

} // end of writeExtraHeaders method




