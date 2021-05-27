#include "CCSDSSegmenter.h"


/*****************************************************************************
* constructor. Max size is the maximum size in bytes of a segmented
* packet including all headers.
*****************************************************************************/
CCSDSSegmenter::CCSDSSegmenter(int max_size) : PacketPipe() {

this->max_size = max_size;
split_packet = new SwiftPacket(max_size);

} // end of constructor


/*****************************************************************************
* return the segmented APID corresponding to an original APID.
*****************************************************************************/
int CCSDSSegmenter::newAPID(int old_apid) {

    if(apid_map.count(old_apid) == 0 ) return old_apid;

    return apid_map[old_apid];

}



/*****************************************************************************
* set the segmented APID corresponding to an original APID.
*****************************************************************************/
void CCSDSSegmenter::setNewAPID(int old_apid, int new_apid) {

apid_map[old_apid] = new_apid;

}



/*****************************************************************************
* get the current sequence counter for a given APID. 
* The counter starts at zero when a new object is created.
*****************************************************************************/
int CCSDSSegmenter::newSequence(int new_apid) {

//cout << "apid="<<new_apid<<" sequence[new_apid]="<<sequence[new_apid]<<"\n";

return (sequence[new_apid])++;

} // end of newSequence methid



/*****************************************************************************
* take a packet, split it up and send the results downstream.
* Note you can only push packets into this pipe. You can't pull them out.
*****************************************************************************/
void CCSDSSegmenter::pushIn(CCSDSPacket* generic_p) {

/*******************************************************
* do nothing if there's no sink to receive the packets *
*******************************************************/
if(sink==NULL) return;

/*****************************************************************
* make sure the packet has a tertiary header
*****************************************************************/
SwiftHead3Packet* p = dynamic_cast<SwiftHead3Packet*>(generic_p);
if(p==NULL) {
    *sink << (CCSDSPacket*)NULL;
    return;
}


cout << "segmenting a packet with size="<<p->size()<<"\n";
/*********************************
* we need to break up the packet *
*********************************/
int size_with_head3 = p->size() + SwiftHead3::size;

if(size_with_head3 >= max_size) split_packet->size(max_size);
else                            split_packet->size(size_with_head3);

/********************************
* set the primary header info *
******************************/
int new_apid = newAPID(p->apid());
split_packet->apid(new_apid);
split_packet->isFirst(1);
split_packet->isLast(0);
split_packet->sequence( newSequence(new_apid) );

/******************************************
* write the tertiary header into the data *
******************************************/
Writer* w = split_packet->dataWriter();
p->head3()->write(w);

/****************************
* fill the rest of the data *
****************************/
unsigned char*   d0 = p->data();
unsigned char* end0 = p->data() + p->size() - 1;

unsigned char* d1 = (split_packet->data()) + SwiftHead3::size;

for( unsigned char* end1 = (split_packet->data()) + split_packet->size() - 1;
     d1 <= end1;
    ) *(d1++) = *(d0++);

cout << "    copied data into first packet\n";

if(size_with_head3 <= max_size) {
    /*****************************************************
    * this will be the only packet - not split after all *
    *****************************************************/
cout << "    only one packet\n";
    split_packet->isLast(1);
    *sink << split_packet;
    return;
}

/****************************************
* loop over new split component packets *
****************************************/
while(1) {
cout << "    writing a segmented packet\n";
    /************************************
    * copy the data into the new packet *
    ************************************/
    unsigned char* end1 = (split_packet->data()) + split_packet->size() - 1;
    while( d1 <= end1 ) {

        /************************
        * copy the current byte *
        ************************/
        *(d1++) = *(d0++);

        /**********************
        * check if we're done *
        **********************/
        if(d0 > end0 ) {
            /**********************************************
            * we've copied all the data, so we should
            * cut this packet to size, send it and quit
            **********************************************/
            split_packet->size(d1 - split_packet->data() );
            split_packet->isLast(1);
            *sink << split_packet;
            return;
        } 

    } // end of loop over bytes

    /*****************************************************
    * send the current packet and get ready for the next *
    *****************************************************/
    *sink << split_packet;
    d1 = split_packet->data();
    split_packet->isFirst(0);
    split_packet->sequence( newSequence(new_apid) );

} // end of loop over packets

    
/***************************
* shouldn't ever get here *
**************************/
cerr << "Programming error in CCSDSSegmenter::pushIn()\n";
exit(1);

    

} // end of push in method

