#include "LDPEncodeBuffer.h"
#include "LDPBuffer.h"
#include "DataBuf.h"
#include <sstream>

/*****************************************************************************
* Constructor. 
* - "p" is a pointer to a prototype LDPPacket. If this is a ChecksumLDPPacket, 
*   then checksums will be included in each packet.
* - "head" is a pointer to the standard LDP header containing the number of
*    pages in the LDP and a Swift tertiary header. Note that if you don't know 
*    the page count a priori, you should set this to zero. The correct page 
*    count will be written in the redundant header at the end.
* - "extra_head" is the non-standard header which should follow the standard 
*    one. If this is a NULL pointer, then only the standard header will be used.
* - "time_increment" is the number of seconds by which to increment the
*   secondary header time in consecutive packets. In real life the secondary
*   header time won't increment so regularly, but this is just a simulation.
*****************************************************************************/
LDPEncodeBuffer::LDPEncodeBuffer(LDPPacket* p, StandardLDPHead* head,
                                 DataBlock* extra_head/*=NULL*/, 
                                 double time_increment/*=0.01*/) 
                 throw(Interpreter::Exception) {

    /***************************************************************
    * This is a cludge to fix how the code broke when going from
    * libstdc++ version 2.x.x to version 3.x.x
    * we set an internal mode flag which is required by 
    * the libstdc++ v3 __copy_streambufs function. 
    * This seems to work, but I'm really just groping in the dark here.
    * The right way to do this would probably be to use setp to
    * equate the buffer with the packet data area. Maybe some day.
    **************************************************************/
    //setp(NULL,NULL); This doesn't do it.
    #ifdef _CPP_BITS_STREAMBUF_TCC
    _M_mode=ios_base::out;
    #endif 


    this->packet = p;
    this->time_increment = time_increment;
    this->size = p->size();
    this->head = head;


    p->page(LDPBuffer::first_page);


    data=packet->data();
    end = data + packet->size();

    /*******************
    * write the header *
    *******************/
    status=WRITING_HEADER;
    ostream* out = new ostream(this);
    Writer* w = new Writer(out);

    head->write(w);

    if(extra_head != NULL) extra_head->write(w);
    status=DONE_WRITING_HEADER;

    delete w;
    delete out;

    count=0;

} // end of constructor


/*****************************************************************************
* write a byte.
* Returns EOF on error and 0 otherwise.
*****************************************************************************/
int LDPEncodeBuffer::overflow(int c /*=EOF*/) {

//cout << "overflow: c = "<<c<<"\n";
++count;

    if(c == EOF ) {
        /*************************************
        * end of file, do closeup operations *
        *************************************/
        close();
        return 0;
    }

    /********************************
    * check if we need a new packet *
    ********************************/
    if(data >= end )  nextPacket();

    /****************
    * copy the data *
    ****************/
    *(data++) = c;
    return 0;


} // end of overflow method

/*****************************************************************************
* update the packet
*****************************************************************************/
void LDPEncodeBuffer::nextPacket() {

//cout << "done with page "<<packet->page()<<" size="<<packet->size()<<"\n";

//cout << "status="<<status<<"\n";

    if(status == WRITING_HEADER || status == DONE_WRITING_HEADER) {
        /*********************************************************
        * save the header packet so that we can write it again
        * at the end of the structure
        *********************************************************/
        LDPPacket* copy = dynamic_cast<LDPPacket*>(packet->copy());
        header_packets.insert(header_packets.end(), copy);

        if(status==DONE_WRITING_HEADER) status = PAST_HEADER;
    }

    /*********************************************
    * finish up the packet - actually calculate  
    * the checksum if there is one.
    *********************************************/
    packet->finish();
    

    /**********************************
    * send the packet down the pipe *
    ********************************/
    _pipe << packet;

    /**********************************
    * increment various header values *
    **********************************/
    packet->page(packet->page()+1);
    packet->time(packet->time() + time_increment);

    packet->sequence(packet->sequence()+1);
    if((int)packet->sequence() == CCSDSPacket::SEQUENCE_PERIOD) {
        packet->sequence(0);
    }

    /**************************
    * reset the data pointers *
    **************************/
    data=packet->data();
    end = data + packet->size();

count=0;


} // end of nextPacket method

/*****************************************************************************
* end the file. This flushes out the last packet being written and
* writes out the redundant header packets.
*****************************************************************************/
void LDPEncodeBuffer::close() {


if(status==CLOSED) return;

// cout << "close\n";
// cout << "count="<<count<<"\n";

/*****************************************************
* truncate the last packet and send it down the pipe *
*****************************************************/
packet->size(data-packet->data());
nextPacket(); // note this increments the page number

/**********************************************************************
* set the status to closed. We have to do this after the nextPacket
* call in case we are still in the header when we close
**********************************************************************/
status=CLOSED;

/*********************************************************************
* set the number of pages in the standard header and re-write this
* into the redundant header packets 
*********************************************************************/
LDPPacket* p = header_packets[0];

if(head->npages()==0 && p->size() >= StandardLDPHead::size ) {

    head->npages(packet->page() - LDPBuffer::first_page );

    ostream* out = new ostream(new DataBuf((char*)(p->data()), p->size()));
    Writer* w    = new Writer(out);

    head->write(w);

    delete w;
    delete out;
}


/******************************************************
* send all the redundant header packets down the pipe *
******************************************************/
for(vector<LDPPacket*>::iterator it = header_packets.begin();
    it != header_packets.end();
    ++it) {

    _pipe << *it;

}


/*************************************************************
* send a null packet down the pipe to indicate end of stream *
*************************************************************/
 _pipe << (CCSDSPacket*)NULL;


} // end of close method
