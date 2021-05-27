#ifndef LDP_PACKET_H
#define LDP_PACKET_H

#include "SwiftPacket.h"
#include "LDPHead3.h"

/*****************************************************************************
* This class represents a packet from the Swift mission, which is part of
* a large data structure. It adds the product and page number header fields
* to the generic SwiftPacket.
*****************************************************************************/
class LDPPacket : public SwiftPacket {

protected:
    /*******
    * data *
    *******/
    LDPHead3* _head3;

protected:
    /*******************
    * copy constructor *
    *******************/
    LDPPacket(const LDPPacket& orig);

public:
    /***************
    * constructors *
    ***************/
    LDPPacket();
    LDPPacket(int size);
    
    /************
    * accessors *
    ************/
    virtual int product() const;
    virtual void product(int product);

    virtual int page() const;
    virtual void page(int product);

    /**********
    * methods *
    **********/
    virtual CCSDSPacket* copy();
    virtual void finish() {}

    /************
    * operators *
    ************/
    virtual int operator<(const CCSDSPacket& generic_right);
    virtual int samePlaceAs(CCSDSPacket* generic_p);




protected:
    /**********
    * methods *
    **********/
    virtual int extraHeadSize() const ;
    virtual void readExtraHeaders( Reader* r) throw(Interpreter::Exception); 
    virtual void writeExtraHeaders(Writer* w) throw(Interpreter::Exception);





}; // end of LDPPacket class


#endif //  LDP_PACKET_H
