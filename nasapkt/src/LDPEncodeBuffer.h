#ifndef LDP_ENCODE_BUFFER_H
#define LDP_ENCODE_BUFFER_H
#include <cstdio>
#include <iostream>
#include <vector>
#include "Writer.h"
#include "PacketPipe.h"
#include "LDPPacket.h"
#include "StandardLDPHead.h"

/*****************************************************************************
* This class is for encoding Swift Large Data Products as CCSDS packets.
* In real life this will be done on-board the spacrcraft, so this class
* is for generating test data. Note that the onboard software may have its
* own quirks.
*
* To use this class you should hook the output drain returned by the pipe()
* method to some chain of processing pipes. Then you probably want to wrap 
* the LDPEncodeBuffer in an ostream.
* Then you can write to the ostream as you would any file. The data will
* be encoded in LDPPackets and sent down the drain pipe.
*
* When you are done writing data you need to explicitly call the close() 
* method.
*****************************************************************************/
class LDPEncodeBuffer : public streambuf {

private:
    /**********************
    * internal class data *
    **********************/
    static const int WRITING_HEADER=0;
    static const int DONE_WRITING_HEADER=1;
    static const int PAST_HEADER=2;
    static const int CLOSED=3;

private:
    /*******
    * data *
    *******/
    StandardLDPHead* head;
    LDPPacket* packet;
    int status;
    vector<LDPPacket*> header_packets;

    int size;
    double time_increment;
    unsigned char* data;
    unsigned char* end; // one past the last valid data byte

    PacketPipe _pipe;

    int count;

public:
    /**************
    * constructor *
    **************/
    LDPEncodeBuffer(LDPPacket* p, StandardLDPHead* head,
                    DataBlock* extra_head=NULL, 
                    double time_increment=0.01) throw(Interpreter::Exception);

    /************
    * accessors *
    ************/
    virtual PacketPipe& pipe() { return _pipe; }

    /**********
    * methods *
    **********/
    virtual void close();


        
    /*****************************
    * streambuf override methods *
    *****************************/
    virtual int overflow(int c = EOF); 

private:
    /*******************
    * internal methods *
    *******************/
    virtual void nextPacket();



}; // end of LDPEncodeBuffer class


#endif // LDP_ENCODE_BUFFER_H
