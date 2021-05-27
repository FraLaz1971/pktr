#ifndef LDP_BUFFER_H
#define LDP_BUFFER_H

#include <cstdio>
#include <iostream>
#include "PacketPipe.h"
#include "LDPPacket.h"

/*****************************************************************************
* This class is for reading the contents of a Swift Large Data Product.
* To use it, you first must connect a stream of LDP packets to the input
* pipe returned by the pipe() method. Then you probably want to 
* create an istream wrapped around the LDPBuffer. At this point you can
* read from the istream just as you would a file containing the decoded
* contents of the LDP. A typical use would be to wrap the istream in a
* Reader object so you can decode binary data.
*
* A streambuf has a limited way of signaling errors. All it can do is 
* report EOF. However, many complicated things can go wrong with a stream
* of LDP packets. Therefore this class uses the following strategy.
* Whenever anything goes wrong it signals EOF. You may then call one
* of the moreData(), newProduct(), missingPages(), or missingBytes() status
* methods to figure out what is going on.
* In some cases you might want to continue reading data after the LDPBuffer
* reports EOF. Then you simply reset the istream status using its clear()
* method.
*
* The apid(), product(), page(), and size() methods and their "prev_" 
* counterparts can be used to get information about the current (or previous)
* packet being read.
*
* This class does not currently support "seeking". In other words each byte
* must be read sequentially.
*
*****************************************************************************/
class LDPBuffer : public streambuf {

public:
    /*************
    * class data *
    *************/
    static const int first_page=1;
    static const int unknown=-1;

private:
    /*******
    * data *
    *******/
    PacketPipe* _pipe;
    LDPPacket* p;
    int position;

    int _npages;
    int _prev_npages;

    double _prev_time;
    int _prev_apid;
    int _prev_product;
    int _prev_page;
    int _prev_size;

    int hit_eof_in_last_block_read;




public:
    /**************
    * constructor *
    **************/
    LDPBuffer();

    /*************
    * destructor *
    *************/
    ~LDPBuffer() { }

    /************
    * accessors *
    ************/
    virtual PacketPipe& pipe() { return *_pipe; }

    virtual int prev_apid()    { return _prev_apid;    }
    virtual int prev_product() { return _prev_product; }
    virtual int prev_page()    { return _prev_page;    }
    virtual int prev_size()    { return _prev_size;    }
    virtual double prev_time()    { return _prev_time;    }

    virtual int apid()    { if(p) return p->apid()   ; else return unknown; }
    virtual int product() { if(p) return p->product(); else return unknown; }
    virtual int page()    { if(p) return p->page()   ; else return unknown; }
    virtual int size()    { if(p) return p->size()   ; else return unknown; }
    virtual double time() { if(p) return p->time()   ; else return 0.0; }

    /*****************
    * status queries *
    *****************/
    virtual int moreData();
    virtual int newProduct();

    virtual int missingPages();
    virtual int missingBytes();


    /*****************************
    * streambuf override methods *
    *****************************/
    virtual int underflow();
    virtual int uflow();


private:
    /********************************
    * methods for internal use only *
    ********************************/
    int nextPacket();


}; // end of LDPBuffer class


#endif // LDP_BUFFER_H
