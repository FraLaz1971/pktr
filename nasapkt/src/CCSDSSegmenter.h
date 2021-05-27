#ifndef CCSDS_SEGMENTER_H
#define CCSDS_SEGMENTER_H

#include "PacketPipe.h"
#include "SwiftHead3Packet.h"
#include <map>

/*****************************************************************************
* This is the opposite of a CCSDSMerger. It takes a stream of Swift
* packets and produces a stream of segmented packets which will be no 
* larger than the size specified in the constructor.
* The segmentation follows the Swift UVOT scheme.
*
* In real life the segmentation is done onboard the spacecraft, so this
* class is mostly useful for generating simulated test data. Even there
* it is of limited usefullness, since the onboard segmentation algorithm
* may have different quirks that this one.
*
* Note you can only push packets into this pipe. If you try to pull one out
* you will just get a NULL pointer indicating end-of-stream.
*****************************************************************************/
class CCSDSSegmenter : public PacketPipe {

private:
    /*******
    * data *
    *******/
    int max_size;

    map<int,int> apid_map;
    map<int,int> sequence;

    SwiftPacket* split_packet;


public:
    /**************
    * constructor *
    **************/
    CCSDSSegmenter(int max_size);

    /*************
    * destructor *
    *************/
    virtual ~CCSDSSegmenter() { 
        delete split_packet;
        split_packet=NULL;
    }



    /************
    * accessors *
    ************/
    virtual int hints() { return 0; }  // hints don't propagate
    virtual void hints(int) {}         // through the segmenter 

    virtual int newAPID(int old_apid);
    virtual void setNewAPID(int old_apid, int new_apid);



    virtual int newSequence(int new_apid);



    /**********
    * methods *
    **********/
    virtual void pushIn(CCSDSPacket* generic_p);
    virtual CCSDSPacket* pullOut() {return NULL; }

}; // end of CCSDSSegmenter class

#endif // CCSDS_SEGMENTER_H
