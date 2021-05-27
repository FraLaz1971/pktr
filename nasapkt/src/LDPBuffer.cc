#include "LDPBuffer.h"

/****************************************************************************
* constructor
****************************************************************************/
LDPBuffer::LDPBuffer() {

/***********************************
* create the PacketPipe connection *
***********************************/
_pipe = new PacketPipe();

/*********************************
* initialize the buffered packet *
*********************************/
p = NULL;
position=0;

/***************************************************
* don't know a priori how many pages there will be *
***************************************************/
_npages =0;

/********************************
* initialize packet information *
********************************/
_prev_apid    = unknown;
_prev_product = unknown;
_prev_page    = unknown;
_prev_size    = unknown;
_prev_time = 0.0;


hit_eof_in_last_block_read=0;


}; // end of constructor


/****************************************************************************
* - Returns 0  if we have read all the packets from the stream
* - Returns 1 otherwise
****************************************************************************/
int LDPBuffer::moreData() { 

    if(p==NULL && _prev_page != unknown ) return 0;
    else                                  return 1;

} // end of moreData method

/****************************************************************************
* - Returns 1 if the current and previous packets were in different products
* - Returns 0 if the current and previous packets were in the same product
* - Returns 1 if the current packet is the first ever read or if no packets
*           have been read.
* - Returns 1 if at the end of the data stream
****************************************************************************/
int LDPBuffer::newProduct() {

if(_prev_page == unknown) return 1; // at beginning
if(p==NULL) return 1;   // at end of packet stream

if((int)p->apid() != _prev_apid || (int)p->product() != _prev_product) return 1;

return 0;

} // end of newProduct method


/****************************************************************************
* Returns the number of pages missing between the packet currently being read
* and the previous packet. If called at EOF, this will tell whether
* a gap was encountered and how large it was (in packets).
* Returns 0 if no packets have been read yet or if a product ended,
* but we didn't know how many pages it should contain.
****************************************************************************/
int LDPBuffer::missingPages() {

if(_prev_page == unknown) {
    /**************************************************
    * this is the first packet we have read 
    * so calculate the gap from the first page number
    **************************************************/
    if(p==NULL) return 0;
    return p->page() - first_page;
}

if(p==NULL || 
   _prev_apid != (int)p->apid() || 
   _prev_product != (int)p->product() ) {
    /*********************************************************
    * the previous packet was the last one in a data product *
    *********************************************************/
    if(_prev_npages==0) return 0; // can't tell if there are any missing
    else                return _npages + first_page -1 - _prev_page;
}

/***************************************************
* if we get here we are in the middle of a product *
***************************************************/
return p->page() - _prev_page -1;

} // end of missingPages method

/****************************************************************************
* Returns the result of missingPages() multiplied by the number of bytes per 
* page.
* It looks at the packets on either side of the gap (if any) to determine
* the page size. 
* Returns LDPBuffer::unknown if the page size can't be determined.
* This method assumes packets have uniform size, except for the last
* non-redundant packet. If this is not true, this method could return
* incorrect results.
****************************************************************************/
int LDPBuffer::missingBytes() {

int npages = missingPages();
if(npages==0) return 0;

if(_prev_size==unknown && p==NULL) return unknown; // no size info available

if(_prev_size==unknown) {
    /**************************************************
    * there was a gap at the beginning of the product *
    **************************************************/

    if(npages != 0 && p->page() == npages +first_page -1 ) {
        /***************************************************************
        * this is the pathological situation where everything in 
        * the product is missing except the last non-redundant packet.
        * Because that packet has odd size, we just return unknown.
        * Note that if the number of pages is not known a priori,
        * we can miss this condition and return an incorrect value.
        * But that will happen rarely, and if it is true we have
        * bigger problems, like we can't read the header.
        ***************************************************************/
        return unknown;
    }

    /*******************************************************
    * use the current packet size - see above for one odd  
    * case where this could be wrong
    *******************************************************/
    return npages * p->size(); 

} // end if gap at the beginning of a product

if(p==NULL || 
   _prev_apid != (int)p->apid() || 
   _prev_product != (int)p->product() ) {
    /*******************************************************************
    * gap at the end - we can't determine the number of bytes missing
    * because the last packet can have any size
    *******************************************************************/
    return unknown;
}

/***********************************************************
* if we get here there is a gap in the middle of a product 
* we will use the size of the previous packet, since the current
* packet might be the last in the product, in which case
* it would have non-representative size
***********************************************************/
return npages* _prev_size;


} // end of missingBytes method

    


/****************************************************************************
* This method is required to implement a streambuf subclass. 
* It gets called whenever someone tries to read past the end
* of the current packet. It resets the internal buffer to the next packet,
* and returns the first byte in that packet as an int. It returns EOF
* if there are any problems.
****************************************************************************/
int LDPBuffer::underflow() { 


hit_eof_in_last_block_read=0; // see LDPBuffer::xsgetn()

/***********************
* read the next packet *
***********************/
if( nextPacket() ) return EOF;

/*********************************************
* reset the buffer into the packet data area *
*********************************************/
char* buffer = (char*)p->data();
setg(buffer, buffer, buffer+p->size());

return (int)buffer[0];

} // end of underflow method

/****************************************************************************
* This is just like the "underflow" method, except that it increments the
* buffer pointer by one to account for the character it returns.
****************************************************************************/
int LDPBuffer::uflow() { 

//cout << "LDPBuffer::uflow called\n";

/***********************
* read the next packet *
***********************/
if( nextPacket() ) return EOF;
 
/*********************************************
* reset the buffer into the packet data area *
*********************************************/
char* buffer = (char*)p->data();
setg(buffer, buffer+1, buffer+p->size());
 
return (int)buffer[0];


} // end of uflow method



/****************************************************************************
* Private method for pulling the next packet out of the pipe.
* Returns 1 if there is anything funny going on between the two packets,
* like a gap or a product number or APID change, or if a NULL packet was
* pulled out of the pipe.
* Returns 0 if everything is fine.
****************************************************************************/
int LDPBuffer::nextPacket() {

//cout << "LDPBuffer::nextPacket\n";

/*************************************
* save the info from the last packet *
*************************************/
if(p!=NULL) {
    _prev_apid    = p->apid();
    _prev_product = p->product();
    _prev_page    = p->page();
    _prev_size    = p->size();
    _prev_time    = p->time();
}

/******************
* read the packet *
******************/
CCSDSPacket* generic_p;
*_pipe >> generic_p;

//cout << " got packet from pipe\n";

p = dynamic_cast<LDPPacket*>(generic_p); // make sure it's an LDP packet
if(p==NULL) return 1;

/*
cout <<  "read packet page="<<p->page()
     << " product="<<p->product()
     << " previous="<<_prev_product
     <<"\n";
*/


/**************************************
* check if anything funny is going on *
**************************************/
if(_prev_apid    != unknown && (int)p->apid()    != _prev_apid   ) return 1;
if(_prev_product != unknown && (int)p->product() != _prev_product) return 1;

if(_prev_page == unknown && p->page() != first_page) return 1;

if(_prev_page != unknown && p->page() - _prev_page != 1 ) {
    /*********************************************************************
    * this page does not follow the last one 
    * this can mean a gap or that we have reached the first page of the 
    * redundant header 
    *********************************************************************/
    if(p->page() == first_page && (_prev_page==_npages || _npages==0) ) {
        /*************************************************
        * this is the first page of the redundant header *
        *************************************************/

        /*****************************************
        * skip over the redundant header packets *
        * by reading packets until we come to a new product
        * or we run out of packets
        *****************************************/
        int saved_apid    = p->apid();
        int saved_product = p->product();

        do {
            CCSDSPacket* generic_p;
            *_pipe >> generic_p;
            p = dynamic_cast<LDPPacket*>(generic_p);

/*
            if(p) 
            cout <<  "read for skipping page="<<p->page()
                 << " product="<<p->product()
                 << "\n";
*/

        } while(p!=NULL && saved_apid    == (int)p->apid() 
                        && saved_product == (int)p->product() );

    } // end if we were in the redundant header

    return 1;

} // end if the pages number were not sequential


/****************************************************
* if we get here, everything is nice and sequential *
****************************************************/
return 0;

} // end of nextPacket method
