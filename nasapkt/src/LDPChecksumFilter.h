#ifndef LDPCHECKSUM_FILTER_H
#define LDPCHECKSUM_FILTER_H

#include "PacketFilter.h"

class LDPChecksumFilter : public PacketFilter {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    LDPChecksumFilter();

    /*************
    * destructor *
    *************/
    virtual ~LDPChecksumFilter() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual bool accept(CCSDSPacket* p);
    
}; // end of LDPChecksumFilter class

#endif // LDPCHECKSUM_FILTER_H
