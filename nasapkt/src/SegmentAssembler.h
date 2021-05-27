
#ifndef SEGMENT_ASSEMBLER_H
#define SEGMENT_ASSEMBLER_H

#include <set>
#include "SwiftHead3Packet.h"
#include "PacketPipe.h"

/****************************************************************************
* This class merges Swift UVOT type packets. A group of packets is specified
* using the bit flags in the CCSDS primary header. Note that although this 
* follows the CCSDS standard, the details of merging are specific to 
* the Swift mission, because they involve handling the secondary and tertiary
* headers.
*
* This class requires all incoming packets to be unique (i.e. not reuse the 
* same pointer), because it stores the packets while waiting to make a
* merged group. This class also assumes that all incoming packets have the
* same APID. It does not assume that the packets are in order.
* General users will probably want to use a CCSDSMerger instead of using this
* class directly, since it handles multiple APIDs and makes unique copies
* of each packet. Note that the output of a SegmentAssembler is not unique
* (i.e. the same packet is reused over and over).
*
* The merged packets will have a different APID from the segmented packets.
* This new APID must be specified when constructing the SegmentAssembler.
*
* This class holds a number of "drain pipes" which can be accessed using the
* scraps(), used(), and duplicates() methods.
* Only complete groups of packets are merged. Leftover, unmerged
* packets are sent down the "scraps" drain. If you push in a packet which
* is the same (i.e. same sequence count and secondary header time) as
* as a packet waiting to be merged, the original packet will be sent
* down the "duplicates" drain.
* All of the packets which comprised complete groups are sent down the
* "used" drain.
****************************************************************************/
class SegmentAssembler {

private:
    /******************************************
    * embedded class used for sorting packets *
    ******************************************/
    struct before {
        bool operator()(SwiftPacket* p1, SwiftPacket* p2) const {
            return comes_before(p1, p2);
        }
    };

private:
    /*******
    * data *
    *******/
    set<SwiftPacket*,before> pool;
    set<SwiftPacket*,before>::iterator position;

    SwiftHead3Packet* merged;

    PacketPipe _scraps;
    PacketPipe _used;
    PacketPipe _duplicates;

    int new_apid;

public:
    /**************
    * constructor *
    **************/
    SegmentAssembler(int new_apid);

    /*************
    * destructor *
    *************/
    virtual ~SegmentAssembler() {}


    /*************
    * accessors *
    ************/
    virtual int poolSize() { return pool.size(); }

    virtual PacketPipe& scraps()     { return _scraps;     }
    virtual PacketPipe& used()       { return _used;       }
    virtual PacketPipe& duplicates() { return _duplicates; }


    /**********
    * methods *
    **********/
    virtual SwiftHead3Packet* addPacket(SwiftPacket* p);
    virtual void flush();



}; // end of SegmentAssembler class

#endif // SEGMENT_ASSEMBLER_H
