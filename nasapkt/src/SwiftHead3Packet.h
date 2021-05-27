#ifndef SWIFT_HEAD3_PACKET_H
#define SWIFT_HEAD3_PACKET_H

#include "SwiftPacket.h"
#include "SwiftHead3.h"

/****************************************************************************
* This class represents a Swift mission packet which includes a standard
* tertiary header.
****************************************************************************/
class SwiftHead3Packet : public SwiftPacket {

private:
    /*******
    * data *
    *******/
    SwiftHead3* _head3;

protected:
    /*******************
    * copy constructor *
    *******************/
    SwiftHead3Packet(const SwiftHead3Packet& orig);


public:
    /***************
    * constructors *
    ***************/
    SwiftHead3Packet();
    SwiftHead3Packet(int size);

    /************
    * accessors *
    ************/
    virtual int       target() const { return _head3->target(); }
    virtual int          obs() const { return _head3->obs(); }
    virtual double   sc_time() const { return _head3->sc_time()->value(); }
    virtual double utcf_time() const { return _head3->utcf_time()->value(); }

    virtual void    target(int    value) { _head3->target(value); }
    virtual void       obs(int    value) { _head3->obs(value); }
    virtual void   sc_time(double value) { _head3->sc_time()->value(value); }
    virtual void utcf_time(double value) { _head3->utcf_time()->value(value); }
    
    virtual double bestTime() { return sc_time(); }

    virtual int obsID() { return (target() << 8) + obs(); }
    virtual void obsID(int value) {
        obs(value & 0xff );
        target(value >> 8 );
    }

    virtual SwiftHead3* head3() { return _head3; }


    /**********
    * methods *
    **********/
    virtual CCSDSPacket* copy();


protected:
    /**********
    * methods *
    **********/
    virtual int extraHeadSize() const;
    virtual void readExtraHeaders( Reader* r) throw(Interpreter::Exception); 
    virtual void writeExtraHeaders(Writer* w) throw(Interpreter::Exception);


    



}; // end of SwiftHead3Packet class

#endif //  SWIFT_HEAD3_PACKET_H
