#include "ChecksumLDPPacket.h"


/*****************************************************************************
* construct an empty packet
*****************************************************************************/
ChecksumLDPPacket::ChecksumLDPPacket() : LDPPacket() {

_trailer = new LDPTrailer();

} // end of empty packet constructor

/*****************************************************************************
* construct a packet with a given number of data bytes
*****************************************************************************/
ChecksumLDPPacket::ChecksumLDPPacket(int size) : LDPPacket(size) {

sizeAccordingToHeader(size);
_trailer = new LDPTrailer();

} // end of packet constructor

/*****************************************************************************
* copy constructor
*****************************************************************************/
ChecksumLDPPacket::ChecksumLDPPacket(const ChecksumLDPPacket& orig) : 
                   LDPPacket(orig) {

sizeAccordingToHeader(orig.size() );


//cout << "ChecksumLDPPacket copy constructor\n";
_trailer = new LDPTrailer();
checksum(orig.checksum());

} // end of copy constructor



/*****************************************************************************
* return the checksum for this packet
*****************************************************************************/
int  ChecksumLDPPacket::checksum() const { return _trailer->checksum(      ); }
void ChecksumLDPPacket::checksum(int checksum) {_trailer->checksum(checksum); }


/***************************************************************************
* virtual wrapper around the copy constructor
***************************************************************************/
CCSDSPacket* ChecksumLDPPacket::copy() { return new ChecksumLDPPacket(*this); }

/***************************************************************************
* calculate the checksum for this packet from its contents.
***************************************************************************/
int ChecksumLDPPacket::calculateChecksum() {

/*************************************
* prepare the checksumming apparatus *
*************************************/
Checksummer* check = new LDPChecksummer();
ostream* stream = new ostream(check);
Writer* w = new Writer(stream);

/***********************************************
* write all but the trailer to the checksummer *
***********************************************/
if(apid() < 1152 ) {
    /*****************************************************
    * The BAT includes the entire packet in the checksum *
    *****************************************************/
    CCSDSHead::write(w);
    writeExtraHeaders(w);
} else {
    /*******************************************************
    * XRT packets do not include the primary or secondary 
    * headers in the checksum
    *******************************************************/
    _head3->write(w);
}

/*************************************************
* both kinds of packets include the data section *
*************************************************/
w->writeBytes(data(), size() );

stream->flush(); // don't know if this is necessary

unsigned long value = check->value();

delete w;
delete stream;
delete check;

return (int)value;

} // end of calculateChecksum method

/***************************************************************************
* this method should be called after all modifications have been made to the 
* packet. It updates the checksum at the end of the packet.
***************************************************************************/
void ChecksumLDPPacket::finish() {

checksum(calculateChecksum());

} // end of finish method



/*****************************************************************************
* methods for handing trailer
*****************************************************************************/
int ChecksumLDPPacket::trailerSize() const { return LDPTrailer::size; }

void ChecksumLDPPacket::readTrailer(Reader* r) throw(Interpreter::Exception) {

_trailer->read(r);

}

void ChecksumLDPPacket::writeTrailer(Writer* w) throw(Interpreter::Exception){

_trailer->write(w);

}
