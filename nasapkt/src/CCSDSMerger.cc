#include "CCSDSMerger.h"

/*****************************************************************************
* constructor
*****************************************************************************/
CCSDSMerger::CCSDSMerger() {

}


/*****************************************************************************
* get the holding tank of packets waiting to be merged for a given apid
*****************************************************************************/
SegmentAssembler* CCSDSMerger::getAssembler(int apid) {

/******************************************************
* check if we already have an assembler for this APID *
******************************************************/
SegmentAssembler* assembler = assemblers[apid];
if(assembler!= NULL ) return assembler;

/*************************
* create a new assembler *
*************************/
assembler = new SegmentAssembler(newAPID(apid));
assemblers[apid]=assembler;

/********************
* hook up the drains *
*********************/
assembler->scraps()     | _scraps;
assembler->used()       | _used;
assembler->duplicates() | _duplicates;

return assembler;

} // end of getAssembler method


/*****************************************************************************
* returns the merged packet APID corresponding to a given segmented packet
* APID.
*****************************************************************************/
int CCSDSMerger::newAPID(int old_apid) {

    if(apid_map.count(old_apid) == 0 ) return old_apid;

    return apid_map[old_apid];

}



/*****************************************************************************
* This method tells the CCSDSMerger about a mapping between a segmented
* APID and the APID of the corresponding merged packets. By default the
* merged packet APID is the same as the segmented packet APID.
*****************************************************************************/
void CCSDSMerger::setNewAPID(int old_apid, int new_apid) {

apid_map[old_apid] = new_apid;

}

/*****************************************************************************
* returns the hints flag for the output packet stream
*****************************************************************************/
//int CCSDSMerger::hints() {
//    int flags = PacketPipe::hints();
//    flags |= PacketPipe::ALL_UNIQUE; // output is always unique
//    return flags;
//}

/*****************************************************************************
* set the hints flag for the incoming packet stream
*****************************************************************************/
void CCSDSMerger::hints(int flags) {

if(flags & PacketPipe::ALL_UNIQUE) unique_packets=1;
else                               unique_packets=0;

PacketPipe::hints(flags);

} // end of set hints method

/*****************************************************************************
* put a segmented packet into the appropriate assembler and return anything
* which pops out
*****************************************************************************/
CCSDSPacket* CCSDSMerger::addPacket(CCSDSPacket* generic_p) {

// cout << "CCSDSMerger::addPacket"
//      <<" apid="<<generic_p->apid()
//      <<" sequence="<<generic_p->sequence()
//      << endl;

/**************************************************************
* cast the packet as a SwiftPacket and copy it if we need to.
* The SegmentAssemblers require unique packets
*************************************************************/
SwiftPacket* p;
if(unique_packets) p = dynamic_cast<SwiftPacket*>(generic_p);
else               p = dynamic_cast<SwiftPacket*>(generic_p->copy());

if(p==NULL) return NULL;

/******************************************************
* put the packet into the appropriate assembler
* if a merged packet pops out, pass it down the line
******************************************************/
return getAssembler( p->apid() )->addPacket(p);


} // end of addPacket method


/*****************************************************************************
* put a segmented packet into the merger
*****************************************************************************/
void CCSDSMerger::pushIn(CCSDSPacket* p) {

if(p==NULL) {
    flushScraps();
    if(sink!=NULL) *sink << (CCSDSPacket*)NULL;
    return;
}

CCSDSPacket* merged = addPacket(p);
if(merged!=NULL && sink!=NULL ) *sink << merged;

} // end of push in method


/*****************************************************************************
* pull out a merged packet. Note it is more natural to push packets in than
* pull them out.
*****************************************************************************/
CCSDSPacket* CCSDSMerger::pullOut() {

//cout << "pulling from merger\n";

if(source==NULL) return NULL;

/************************************************************************
* pull packets from the source and process them until we get a merged
* one or we run out of source packets
************************************************************************/
for(;;) {

    CCSDSPacket* p;
    *source >> p;
    if(p==NULL) {
        flushScraps();
        return NULL;
    }

    CCSDSPacket* merged = addPacket(p);
    if(merged!=NULL) return merged;

} // end of loop over input packets

} // end of pullOut method

/*****************************************************************************
* flush the scraps from all the assemblers
*****************************************************************************/
void CCSDSMerger::flushScraps() {

//cout << "CCSDSMerger::flushScraps: start\n";


    for(map<int,SegmentAssembler*>::iterator it=assemblers.begin();
        it != assemblers.end();
        ++it ) {
        SegmentAssembler* assembler = it->second;

        assembler->flush();

    } // end of loop over assemblers

    _scraps << (CCSDSPacket*)NULL;

} // end of flushScraps method
