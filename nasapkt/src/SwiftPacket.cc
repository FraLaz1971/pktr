#include "SwiftPacket.h"
#include <math.h>

/*****************************************************************************
* construct an empty packet
*****************************************************************************/
SwiftPacket::SwiftPacket() : CCSDSPacket() {

_head2 = new SwiftHead2();

} // end of empty packet constructor

/*****************************************************************************
* construct a packet with a given number of data bytes.
*****************************************************************************/
SwiftPacket::SwiftPacket(int size) : CCSDSPacket(size) {

sizeAccordingToHeader(size);

_head2 = new SwiftHead2();


} // end of packet constructor

/*****************************************************************************
* copy constructor
*****************************************************************************/
SwiftPacket::SwiftPacket(const SwiftPacket& orig) : CCSDSPacket(orig) {

sizeAccordingToHeader(orig.size() );

//cout << "SwiftPacket copy constructor size="<<size()<<"\n";

_head2= new SwiftHead2();
time(orig.time());

} // end of copy constructor



/*****************************************************************************
* secondary header time accessors
*****************************************************************************/
double SwiftPacket::time(     ) const { return _head2->time()->value    (); }
void   SwiftPacket::time(double time) {        _head2->time()->value(time); }

/*****************************************************************************
* returns the time best reflecting the data in the packet.
* HEAD3 packets override this method to return the tertiary header time.
*****************************************************************************/
double   SwiftPacket::bestTime() {  return time(); }

/*****************************************************************************
* return a pointer to a new copy of this packet cast as a generic CCSDSPacket
* This is basicly a virtual wrapper around the copy constructor
*****************************************************************************/
CCSDSPacket* SwiftPacket::copy() { return new SwiftPacket(*this); }


/****************************************************************************
* returns true if two packets have the same time and sequence values 
****************************************************************************/
int SwiftPacket::samePlaceAs(CCSDSPacket* generic_p) {

SwiftPacket* p = dynamic_cast<SwiftPacket*>(generic_p);
if(p==0) return CCSDSPacket::samePlaceAs(generic_p);

// This is inefficient, but a safe check to have.
// Under Linux I was getting different double time values for the same
// underlying integers in different calls.
// maybe some day we can remove this
if( time() != p->time()  && fabs(time() -p->time() ) < 1e-5 ) {
    cerr << "Warning bad double comparison in SwiftPacket::samePlaceAs\n";
}


return time() == p->time() && sequence() == p->sequence();

} // end of samePlaceAs method

/*****************************************************************************
* ordering operator
* order the packets by increasing time and sequence counter.
*****************************************************************************/
int SwiftPacket::operator<(const CCSDSPacket& generic_right) {

SwiftPacket const *  right   = dynamic_cast<SwiftPacket const *>(&generic_right);

if(right==NULL) return CCSDSPacket::operator<(generic_right);

//cout << "comparing by time\n";

double  left_time=time();
double right_time=right->time();

if(left_time != right_time ) return (left_time < right_time);

return CCSDSPacket::operator<(generic_right);

} // end of less than operator





/*****************************************************************************
* size of the secondary header in bytes
*****************************************************************************/
int SwiftPacket::extraHeadSize() const { return SwiftHead2::size; }


/*****************************************************************************
* read the secondary header
*****************************************************************************/
void SwiftPacket::readExtraHeaders(Reader* r) throw(Interpreter::Exception) {

    _head2->read(r);

} // end of readExtraHeaders method

/*****************************************************************************
* write the secondary header
*****************************************************************************/
void SwiftPacket::writeExtraHeaders(Writer* w) throw(Interpreter::Exception) {

    _head2->write(w);

} // end of writeExtraHeaders method



