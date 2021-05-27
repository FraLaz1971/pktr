#include "LDPPacket.h"

/*****************************************************************************
* construct an empty packet
*****************************************************************************/
LDPPacket::LDPPacket() : SwiftPacket() {

_head3 = new LDPHead3();

} // end of empty packet constructor

/*****************************************************************************
* construct a packet with a given number of data bytes
*****************************************************************************/
LDPPacket::LDPPacket(int size) : SwiftPacket(size) {

sizeAccordingToHeader(size);
_head3 = new LDPHead3();

} // end of packet constructor

/*****************************************************************************
* copy constructor
*****************************************************************************/
LDPPacket::LDPPacket(const LDPPacket& orig) : SwiftPacket(orig) {

//cout << "LDPPacket copy constructor\n";

sizeAccordingToHeader(orig.size() );


_head3 = new LDPHead3();

product(orig.product());
page(orig.page());

} // end of copy constructor




/*****************************************************************************
* accessors
*****************************************************************************/
int  LDPPacket::product(     ) const { return _head3->product(       ); }
void LDPPacket::product(int product) {        _head3->product(product); }

int  LDPPacket::page(     ) const { return _head3->page(       ); }
void LDPPacket::page(int product) {        _head3->page(product); }


/***************************************************************************
* virtual wrapper around the copy constructor
***************************************************************************/
CCSDSPacket* LDPPacket::copy() { return new LDPPacket(*this); }

/*****************************************************************************
* ordering operator
* order the packets by increasing product number and then page number
* and then by time and sequence counter
*****************************************************************************/

int LDPPacket::operator<(const CCSDSPacket& generic_right) {

//cout << "comparing LDP packets\n";

LDPPacket const *  right   = dynamic_cast<LDPPacket const *>(&generic_right);
if(right==NULL) return SwiftPacket::operator<(generic_right);

if(product() != right->product()) return product() < right->product();

/***********************
* compare page numbers *
***********************/
int  left_page =        page();
int right_page = right->page();
if(left_page    != right_page   ) return (left_page < right_page);

return SwiftPacket::operator<(generic_right);

} // end of less than operator

/****************************************************************************
* returns true if two packets have the same time and sequence values 
****************************************************************************/
int LDPPacket::samePlaceAs(CCSDSPacket* generic_p) {

//cout << "LDP samePlaceAs\n";

LDPPacket* p = dynamic_cast<LDPPacket*>(generic_p);
if(p==0) return SwiftPacket::samePlaceAs(generic_p);

return product() == p->product() && page() == p->page();

} // end of samePlaceAs method




/***************************************************************************
* add the LDP product and page numbers to the headers
***************************************************************************/
int LDPPacket::extraHeadSize() const { 

    return SwiftPacket::extraHeadSize() + LDPHead3::size;

} // end of extraHeadSize method


/*******
* read *
*******/
void LDPPacket::readExtraHeaders( Reader* r) throw(Interpreter::Exception) {

SwiftPacket::readExtraHeaders(r);
_head3->read(r);

} // end of readExtraHeaders method

/********
* write *
********/
void LDPPacket::writeExtraHeaders(Writer* w) throw(Interpreter::Exception) {

SwiftPacket::writeExtraHeaders(w);
_head3->write(w);

} // end of writeExtraHeaders method
