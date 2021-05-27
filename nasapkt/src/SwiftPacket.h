#ifndef SWIFT_PACKET_H
#define SWIFT_PACKET_H

#include "CCSDSPacket.h"
#include "SwiftHead2.h"

/****************************************************************************
* This class represents a generic packet from the Swift mission.
* It adds a Swift secondary header to the generic CCSDS packet.
****************************************************************************/
class SwiftPacket : public CCSDSPacket {

private:
    /*******
    * data *
    *******/
    SwiftHead2* _head2;

protected:
    /*******************
    * copy constructor *
    *******************/
    SwiftPacket(const SwiftPacket& orig);

public:
    /***************
    * constructors *
    ***************/
    SwiftPacket();
    SwiftPacket(int size);
    
    /************
    * accessors *
    ************/
    virtual double time() const;
    virtual void   time(double time);
    virtual double bestTime();

    /**********
    * methods *
    **********/
    virtual CCSDSPacket* copy();

    /************
    * operators *
    ************/
    virtual int operator<(const CCSDSPacket& right);
    virtual int samePlaceAs(CCSDSPacket* generic_p);


protected:
    /**********
    * methods *
    **********/
    virtual int extraHeadSize() const;
    virtual void readExtraHeaders( Reader* r) throw(Interpreter::Exception); 
    virtual void writeExtraHeaders(Writer* w) throw(Interpreter::Exception);


}; // end of SwiftPacket class

#endif //  SWIFT_PACKET_H
