#ifndef LDPPACKET_STUB_H
#define LDPPACKET_STUB_H

#include <stdexcept>
using namespace std;


#include "Sequence.h"
#include "LDPStub.h"
#include "LDPPacket.h"




class LDPPacketStub {

public:
    static const int MAX_LDP_PAGES=65535; // this is the max that for 2 bytes
    constexpr static const double MAX_LDP_DURATION=24.*2600.; // one day
    constexpr static const double MAX_TIME_PER_PACKET=3600.; // seconds
    
    /*********************************************************
    * exception thrown when we can't tell if two packets are
    * from the same LDP
    *********************************************************/
    class MaybeException : public runtime_error {

        public:
            MaybeException(const string& context) : runtime_error(context) { }
            MaybeException() : runtime_error("") { }

    }; // end of MaybeException class


private:
    /*******
    * data *
    *******/
    bool is_xrt;
    double time;
    Sequence seq;

    int product;
    int page;
    int npages;

    LDPStub* ldp;




public:
    /**************
    * constructor *
    **************/
    LDPPacketStub(LDPPacket* packet);

    /*************
    * destructor *
    *************/
    virtual ~LDPPacketStub() {}

    /************
    * accessors *
    ************/
    virtual LDPStub* getLDP();
    virtual void setLDP(LDPStub* ldp);

    virtual int getLDPPageCount();
    
    virtual int getProduct();

    /**********
    * methods *
    **********/
    virtual bool sameLDPAs(LDPPacketStub* stub) throw(MaybeException);

    /************
    * operators *
    ************/
    friend bool operator<(const LDPPacketStub& left, const LDPPacketStub& right);
    friend bool operator==(const LDPPacketStub& left, const LDPPacketStub& right);

}; // end of LDPPacketStub class

#endif // LDPPACKET_STUB_H
