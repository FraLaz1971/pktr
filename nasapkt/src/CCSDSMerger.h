#ifndef CCSDS_MERGER_H
#define CCSDS_MERGER_H

#include "PacketTee.h"
#include "SegmentAssembler.h"
#include <map>

/****************************************************************************
* This class contains a number of SegmentAssembler objects in order
* to merge a stream of Swift UVOT-style packets containing multiple APIDs.
*
* Unlike SegmentAssembler, this class is a PacketPipe. You may push
* segmented packets in and merged packets will come out the other end.
* You can also pull them out, but it is more efficient to push them in.
*
* After creating a CCSDSMerger, you have to call the setNewAPID() method
* to teach the merger what merged packet APIDs correspond to which segmented
* packet APIDs.
*
* This class has the same drain pipes as SegmentAssembler. Each of these pipes
* collects the outflow from the coresponding drain pipe of all the 
* SegmentAssembler classes.
****************************************************************************/
class CCSDSMerger : public PacketPipe {

private:
    /*******
    * data *
    *******/
    map<int,SegmentAssembler*> assemblers;
    map<int,int> apid_map;
    int unique_packets;

    PacketTee _scraps;
    PacketTee _used;
    PacketTee _duplicates;

public:
    /**************
    * constructor *
    **************/
    CCSDSMerger();

    /*************
    * destructor *
    *************/
    virtual ~CCSDSMerger() {}


    /************
    * accessors *
    ************/
    SegmentAssembler* getAssembler(int apid);

    virtual int newAPID(int old_apid);
    virtual void setNewAPID(int old_apid, int new_apid);

//    virtual int hints();
    virtual void hints(int);

    virtual PacketPipe& scraps()     { return _scraps;     }
    virtual PacketPipe& used()       { return _used;       }
    virtual PacketPipe& duplicates() { return _duplicates; }



    /**********
    * methods *
    **********/
    virtual CCSDSPacket* addPacket(CCSDSPacket* generic_p);

    virtual void pushIn(CCSDSPacket* p);
    virtual CCSDSPacket* pullOut();

private:
    /**********************
    * internal use method *
    **********************/
    virtual void flushScraps();  

}; // end of CCSDSMerger class

#endif // CCSDS_MERGER_H
