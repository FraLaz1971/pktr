#include "CCSDSPacket.h"

#include "DataBuf.h"
#include <sstream>

/*****************************************************************************
* constructor - Creates a packet with zero data size. Note that by CCSDS rules
* a packet must have at least one data byte, so the size(int) method should
* be called before anything serious is done with the packet.
*****************************************************************************/
CCSDSPacket::CCSDSPacket() : CCSDSHead() {

version(0);
type(0);
hasHead2(1);
isFirst(1);
isLast(1);

apid(0);
sequence(0);

initDataArray();


} // end of constructor


/*****************************************************************************
* constructor - Create a packet with a given number of data bytes.
*****************************************************************************/
CCSDSPacket::CCSDSPacket(int size) : CCSDSHead() {

version(0);
type(0);
hasHead2(1);
isFirst(1);
isLast(1);

apid(0);
sequence(0);


initDataArray();
this->size(size);

} // end of constructor from scratch

/*********************************************************************
* copy constructor
*********************************************************************/
CCSDSPacket::CCSDSPacket(const CCSDSPacket& orig) : CCSDSHead(orig) {

//cout << "CCSDSPacket copy constructor\n";

/******************************
* set the size to be the same *
******************************/
int data_size = orig.size();

initDataArray();
size(data_size);

/****************
* copy the data *
****************/
unsigned char* d1=_data;
unsigned char* d0=orig.data();
for(int i=0; i< data_size; ++i) {
    *(d1++) = *(d0++);
}

    

} // end of copy constructor

/*************************************************************************
* destructor - frees storage allocated for the data array
*************************************************************************/
CCSDSPacket::~CCSDSPacket() {

//cout << "CCSDSPacket destructor\n";

resizeDataArray(0);


} // end of destructor

/*************************************************************************
* set up the data array as being empty 
*************************************************************************/
void CCSDSPacket::initDataArray() {

    _data=NULL;
    array_size=0;

} // end of initDataArray method


/*************************************************************************
* resizes the data byte array to a specific size.
* Note this is for internal use only - it does not set the 
* header length field
*************************************************************************/
void CCSDSPacket::resizeDataArray(int size) {

/***************************************************
* free the data if we are setting the size to zero *
***************************************************/
if(size==0) {
    free(_data);
    _data=NULL;
    array_size=0;

}

/***************************************************************************
* don't ever make the array smaller - this should improve efficiency in 
* practice, since we often want to make the same packet larger again later
**************************************************************************/
if(size<=array_size) return;

/***********************************************************
* reallocate the array storage - possibly moving the data
* already in it
***********************************************************/
_data = (unsigned char*)realloc(_data,size);
array_size=size;

} // end of resizeDataArray method


/*********************************************************************
* return the data field size which is recorded in the primary header
*********************************************************************/
int CCSDSPacket::sizeAccordingToHeader() const {

return CCSDSHead::length() + 1 - extraSize();

} // end of sizeAccordingToHeader method

/*********************************************************************
* set the data field size in the primary header
* this method is for internal use only since it does not ensure
* that the data array is large enough to hold this many bytes.
*********************************************************************/
void  CCSDSPacket::sizeAccordingToHeader(int size) {

    int length = size - 1 + extraSize();
    if(length<0) length=0;
    if(length>MAX_LENGTH)length=MAX_LENGTH;
    CCSDSHead::length(length);

} // end of sizeAccordingToHeader method


/*******************************************************************
* return the number of bytes in the data field
*******************************************************************/
int CCSDSPacket::size() const { 

 if(_data) return sizeAccordingToHeader(); 
 else      return 0;

} // end of size method

/*******************************************************************
* resize the data field to a given number of bytes
*******************************************************************/
void CCSDSPacket::size(int size) {

//cout << "setting size to "<<size<<" old size="<<this->size()<<"\n";

sizeAccordingToHeader(size);
resizeDataArray(size);

} // end of size method

/********************************************************************
* override the CCSDSHeader versions of the length method to 
* tie it to the data array size
********************************************************************/
void CCSDSPacket::length(unsigned int value) { size(value+1-extraSize()); }
unsigned int CCSDSPacket::length() const { return CCSDSHead::length(); }


/************************************************************************
* return a pointer to a Reader object pointing to the data block of this
* packet.
* This is a new object which should be deleted when you are done
* with it.
************************************************************************/
Reader* CCSDSPacket::dataReader() {

    return new Reader( new istream(new DataBuf((char*)_data, size())));

 //   return new Reader( new istrstream((char*)_data, size() ));

}

/************************************************************************
* return a pointer to a Reader object pointing to the data block of this
* packet. This is a new object which should be deleted when you are done
* with it.
************************************************************************/
Writer* CCSDSPacket::dataWriter() {

  //  return new Writer( new ostrstream((char*)_data, size() ));

    return new Writer( new ostream(new DataBuf((char*)_data, size())));

}



/*********************************************************************
* returns the minimum allowed value of the length field in the 
* primary headewr for this type of packet. 
*********************************************************************/
int CCSDSPacket::minimumLength() {

    return extraHeadSize() + trailerSize() -1;

} // end of minimumLength method



/********************************************************************
* read the contents of the packet which follows the primary header.
* Note the primary header must be set (particularly the "length" field
* before this method can be called.
********************************************************************/
void CCSDSPacket::readBody(Reader* r) throw(Interpreter::Exception) {

//cout << "CCSDSPacket::readBody start\n";

    /************************************************
    * read any additional (e.g. secondary) headers
    * this does nothing for this class but 
    * subclasses may define something different
    ************************************************/
    if(hasHead2() ) readExtraHeaders(r);

    /****************
    * read the data *
    ****************/
    int data_size = sizeAccordingToHeader();
    resizeDataArray(data_size);

    r->readBytes(_data, data_size, data_size);

    /*************************************
    * read the trailer (if there is one) *
    *************************************/
    readTrailer(r);


} // end of readBody method


/***********************************************************************
* copy the given primary header into the packet and then read the
* rest of the packet. This method is useful if you want to read the
* packet in different ways depending on the APID. 
***********************************************************************/
void CCSDSPacket::read(const CCSDSHead& head, Reader* r) 
                  throw(Interpreter::Exception) {

    /******************
    * copy the header *
    ******************/
    version( head.version() );
    type(    head.type()    );
    hasHead2(head.hasHead2());
    apid(    head.apid()    );
    isFirst( head.isFirst() );
    isLast(  head.isLast()  );
    sequence(head.sequence());
    CCSDSHead::length( head.length() ); 

    /******************************
    * read the rest of the packet *
    ******************************/
    readBody(r);



} // end of read after a given header method


/********************************************************************
* read the entire packet
********************************************************************/
void CCSDSPacket::read(Reader* r) throw(Interpreter::Exception) {
    
    /**************************
    * read the primary header *
    **************************/
    CCSDSHead::read(r);

    /******************************
    * read the rest of the packet *
    ******************************/
    readBody(r);


} // end of read method

/********************************************************************
* write the entire packet
********************************************************************/
void CCSDSPacket::write(Writer* w) throw(Interpreter::Exception) {

   CCSDSHead::write(w);
   if(hasHead2()) writeExtraHeaders(w);
   w->writeBytes(_data, size() );
   writeTrailer(w);

} // end of write method

/************************************************************************
* override the CCSDS head method since we don't need all those values
************************************************************************/
const string& CCSDSPacket::toString() {

    ostringstream s;
    s << "apid=" << apid();
    s << " isFirst=" << isFirst();
    s << " isLast=" << isLast();
    s << " sequence=" << sequence();
    s << " full size=" << length()+7;
    s << " data size=" << size();
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();

    return line;

} // end oif toString method



/************************************************************************
* return a pointer to an exact copy of this packet stored in completely
* different memory. Subclasses should override this method to
* copy secondary header data as well.
* This is basicly a virtual funtion wrapper around the copy constructor
************************************************************************/
CCSDSPacket* CCSDSPacket::copy() { return new CCSDSPacket(*this); }




/***************************************************************************
* these are methods for comparing pointers for sorting and uniquing
* Useful if you have a STL container full of pointers to packets
***************************************************************************/
int comes_before(CCSDSPacket* left, CCSDSPacket* right) {

    return (*left < *right);

}  // end of comes before function to be used for sorting

/****************************************************************************
* this function should be called when uniquing an STL container-full of
* pointers to packets. It is a wrapper for a virtual method
*****************************************************************************/
int unique_comparator(CCSDSPacket* left, CCSDSPacket* right) {

    return left->samePlaceAs(right);

} // end of "equals" comparator for uniquing

/****************************************************************************
* this is the virtual funtion called by unique_comparator
* it returns true if two packets are not unique.
****************************************************************************/
int CCSDSPacket::samePlaceAs(CCSDSPacket* p) { 
    return sequence() == p->sequence(); 
}



/*****************************************************************************
* ordering operator. Order packets by increasing sequence counter.
* Note that the sequence counter is cyclical, so this comparison does not 
* make sense over a range of more than SEQUENCE_PERIOD/2 packets.
* This method correctly handles rollover.
****************************************************************************/
int CCSDSPacket::operator<(const CCSDSPacket& right) {

//cout << "comparing by sequence\n";

int diff = sequence() - right.sequence();

if(diff >  CCSDSPacket::SEQUENCE_HALF_PERIOD ) return 1;
if(diff < -CCSDSPacket::SEQUENCE_HALF_PERIOD ) return 0;

return (diff<0);

} // end of less than operator

/********************************************************************
* returns true if the sequence counters indicate the two packets are
* consecutive in a time sorted stream.
* Returns false if p==NULL;
********************************************************************/
int CCSDSPacket::isAdjacentTo(CCSDSPacket* p) {

if(p==NULL) return 0;

int diff = abs((long int)sequence() - (long int)p->sequence());

//cout << "    isAdjacentTo: diff="<<diff<<"\n";

if(diff == 1 || diff== SEQUENCE_PERIOD-1) return 1;
else                                      return 0;

} // end of isAdjacentTo method


/********************************************************************
* This class has no secondary header, but subclasses may add one
* by overriding these three methods
********************************************************************/
int CCSDSPacket::extraHeadSize() const { return 0; }
void CCSDSPacket::readExtraHeaders(Reader* r) throw(Interpreter::Exception) {}
void CCSDSPacket::writeExtraHeaders(Writer* w) throw(Interpreter::Exception) {}

/********************************************************************
* This class has no trailer, but subclasses may add one
* by overriding these three methods
********************************************************************/
int CCSDSPacket::trailerSize() const { return 0; }
void CCSDSPacket::readTrailer(Reader* r) throw(Interpreter::Exception) {}
void CCSDSPacket::writeTrailer(Writer* w) throw(Interpreter::Exception) {}

/**************************************************************************
* extra header and trailer size combined
**************************************************************************/
int CCSDSPacket::extraSize() const { return extraHeadSize() + trailerSize(); }

