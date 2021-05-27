#ifndef LDPSTUB_REPOSITORY_H
#define LDPSTUB_REPOSITORY_H

#include <set>
#include <map>

#include "LDPPacketStub.h"

class LDPStubRepository {

private:
    /************************************************
    * embedded comparison class for sorting the set *
    ************************************************/
    class lessThan {
        public:
        bool operator()(const LDPPacketStub* left, const LDPPacketStub* right) {
            return *left < *right;
        }
    }; // end of embedded class for comparisons

private:
    /*******
    * data *
    *******/
    multiset<LDPPacketStub*, lessThan> s;
    multiset<LDPPacketStub*, lessThan>::iterator position;
    
    map<int, int> next_index;


public:
    /**************
    * constructor *
    **************/
    LDPStubRepository();

    /*************
    * destructor *
    *************/
    virtual ~LDPStubRepository() {}

    /************
    * accessors *
    ************/
    virtual int getNextIndex(int product);

    /**********
    * methods *
    **********/
    virtual bool addPacket(LDPPacketStub* stub);
    virtual LDPStub* addPacket(LDPPacket* packet);
    void initWith(LDPPacket* packet, int index);

}; // end of LDPStubRepository class

#endif // LDPSTUB_REPOSITORY_H
