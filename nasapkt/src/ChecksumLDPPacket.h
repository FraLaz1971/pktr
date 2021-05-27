#ifndef CHECKSUM_LDP_PACKET_H
#define CHECKSUM_LDP_PACKET_H
#include <cstdio>
#include "LDPPacket.h"
#include "LDPTrailer.h"
#include "LDPChecksummer.h"

/*****************************************************************************
* This class represents an LDPPacket with a checksum included at the end.
*****************************************************************************/
class ChecksumLDPPacket : public LDPPacket {

private:
    /*******
    * data *
    *******/
    LDPTrailer* _trailer;

protected:
    /*******************
    * copy constructor *
    *******************/
    ChecksumLDPPacket(const ChecksumLDPPacket& orig);


public:
    /***************
    * constructors *
    ***************/
    ChecksumLDPPacket();
    ChecksumLDPPacket(int size);
    
    /************
    * accessors *
    ************/
    virtual int checksum() const;
    virtual void checksum(int checksum);

    /**********
    * methods *
    **********/
    virtual CCSDSPacket* copy();
    virtual int calculateChecksum();
    virtual void finish();


protected:
    /**********
    * methods *
    **********/
    virtual int trailerSize() const;
    virtual void  readTrailer(Reader* r) throw(Interpreter::Exception);
    virtual void writeTrailer(Writer* w) throw(Interpreter::Exception);

}; // end of ChecksumLDPPacket class

#endif //  CHECKSUM_LDP_PACKET_H
