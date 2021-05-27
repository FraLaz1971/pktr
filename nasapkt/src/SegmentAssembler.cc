#include "SegmentAssembler.h"

#include "DataBuf.h"
#include <sstream>

/*****************************************************************************
* constructor
*****************************************************************************/
SegmentAssembler::SegmentAssembler(int new_apid) {

    position = pool.end();
    this->new_apid = new_apid;

    merged = new SwiftHead3Packet();

}


/*****************************************************************************
* add a packet to the pool, if it makes a complete set, remove that set from 
* the pool and return a merged version of it.
*****************************************************************************/
SwiftHead3Packet* SegmentAssembler::addPacket(SwiftPacket* p) {


// cout << "new_apid="<<new_apid<<" size="<<pool.size()<<endl;
// 
// 
// cout << "SegmentAssembler::addPacket start apid="<<p->apid()
//     << " sequence="<<p->sequence()
//     <<endl;


/*****************************
* add the packet to the pool *
*****************************/
int original_size = pool.size();
position = pool.insert(position, p);

if((int)pool.size() == original_size) {
    /************************************************************
    * packet is a duplicate of one already in the pool
    * so delete the old packet and replace it with the new one
    ************************************************************/
//cout << "    duplicate of packet in pool already pool size ="<<pool.size()<<endl;
    _duplicates << *position;

    pool.erase(position);
    position = pool.insert(pool.end(), p);

}


//cout << "    "<<pool.size() << " packets in pool"<<endl;

/*******************************************
* search back to find the beginning packet *
*******************************************/
set<SwiftPacket*,before>::iterator start = position;
set<SwiftPacket*,before>::iterator previous = start;
for(--previous;
    !(*start)->isFirst()    &&
    start != pool.begin() &&
    (*previous)->isAdjacentTo(*start) ;
    --previous, --start ) {
}


/************************************************************
* if we couldn't find the beginning, return a null pointer 
* since we don't have a full set
************************************************************/
if(!(*start)->isFirst() ) return NULL;

/***************************************
* search ahead to find the last packet *
***************************************/
//cout << "    about to look for end\n";

//cout << "    (*position)->sequence()="<<(*position)->sequence()<<"\n";

set<SwiftPacket*,before>::iterator end = position;
previous=position;
for( ++end;
    !(*previous)->isLast()  && 
    end != pool.end()      &&
    (*previous)->isAdjacentTo(*end) ;
     ++previous, ++end) {
/*

    cout <<"        searching... end@" << *end
         << "(*end)->sequence()=" << (*end)->sequence()
         << "apid="<<(*end)->apid()
         << "\n";

    cout <<"                previous@" << *previous
         << "(*previous)->sequence()=" << (*previous)->sequence()
         << "\n";

    if(end == pool.end() ) { cout << "        at end\n"; }


exit(1);
*/
}

//cout << "    found_last_packet\n";

/************************************************************
* if we couldn't find the end, return a null pointer 
* since we don't have a full set
************************************************************/
if( ! (*previous)->isLast() ) return NULL;

/****************************************************
* if we got here, then we found a complete set
* so merge it and delete it from the pool
* first calculate the total data size
****************************************************/
//cout << "    merging\n";
int size = -SwiftHead3::size;
for(set<SwiftPacket*,before>::iterator it = start;
    it != end;
    ++it) {

    size += (*it)->size();
//cout << "        partial size="<< (*it)->size()<<"\n";

}

//cout << "        total size="<<size<<"\n";

/***************************************************
* make sure the marged packet has the correct size *
***************************************************/
//SwiftHead3Packet* merged = new SwiftHead3Packet(size);
merged->size(size);

/********************************
* set the primary header values *
********************************/
p = *(start);
merged->version(p->version() );
merged->type(p->type());
merged->hasHead2(1);
merged->apid( new_apid );
merged->isFirst(1);
merged->isLast(1);
merged->sequence(p->sequence());

/**********************************
* set the secondary header values *
**********************************/
merged->time(p->time());

/***************************
* read the tertiary header *
***************************/
int n_bytes_eaten_by_head3=0;
if(p->size() >= SwiftHead3::size ) {
    /***********************************************
    * can read entire header from the first packet *
    ***********************************************/
//cout << "        reading head3 from first packet\n";
    merged->head3()->read( p->dataReader() );
    n_bytes_eaten_by_head3=SwiftHead3::size;

} else {
    /************************************************************
    * the tertiary header is spread over multiple packets 
    * This is sort of cludgey, but we will copy the data
    * from multiple packets into an array and then read the 
    * header from that
    ************************************************************/
// cout << "        head3 spread over multiple packets sequence="<<p->sequence()<<"\n";
//cout << "        SwiftHead3::size = "<<SwiftHead3::size<<"\n";
    unsigned char* array = new unsigned char[SwiftHead3::size];
    for(int i=0; i< SwiftHead3::size; ) {

//cout << "            i="<<i<<" p@"<<p<<"\n";

        /****************************************
        * copy the data from the current packet *
        ****************************************/
        unsigned char* data = p->data();
        for(n_bytes_eaten_by_head3=0 ; 
            n_bytes_eaten_by_head3< p->size() && i<SwiftHead3::size; 
            ++i, ++n_bytes_eaten_by_head3) {

            array[i] = data[n_bytes_eaten_by_head3];
        }

//cout << "            copied from current packet\n";

        /*******************************************************************
        * get another packet if we aren't done collecting the header bytes *
        *******************************************************************/
        if(i<SwiftHead3::size) {
           
            /************************************************************
            * we delete the packet we just read now, because we will
            * increment the "start" iterator, so later when we delete
            * packets from start - end it won't be deleted
            ************************************************************/
            _used << p;

            set<SwiftPacket*,before>::iterator orig=start;
            ++start;
            pool.erase(orig,start);

            p = *start;
        } // end if we need another packet

//cout << "            end of loop\n";

    } /* end of loop copying head3 bytes */

//cout << "        copied head3 to array\n";

    /*******************************
    * now read the tertiary header *
    *******************************/
    istream* stream = new istream(new DataBuf((char*)array, SwiftHead3::size));
    Reader* r = new Reader(stream);
    merged->head3()->read(r);
    delete stream;
    delete r;
    delete array;

        

} // end if head3 was spread over multiple packets


/**************************************
* copy the remaining data from the  
* first packet into the merged packet
**************************************/
unsigned char* d1 = merged->data();
unsigned char* d0     = p->data() + n_bytes_eaten_by_head3;
unsigned char* d0_end = p->data() + p->size();

while(d0 != d0_end ) {
   *(d1++) = *(d0++);
}

/*****************************************
* now loop over the rest of the packets 
* and merge their data as well 
*****************************************/
set<SwiftPacket*,before>::iterator it = start;
++it;
for(;  it != end;  ++it) {

    p = *it;

    d0     = p->data();
    d0_end = p->data() + p->size();

    while(d0 != d0_end ) {
        *(d1++) = *(d0++);
    }

} // end of loop over packets
    

/***********************************
* delete the packets from the pool *
***********************************/
for(it=start; it!=end; ++it) {
    _used << *(it);
}
pool.erase(start,end);
position=end;

//cout << "    done merging\n";

return merged;
       

} // end of addPacket method


/*****************************************************************************
* take all the packets from the pool and send them down the scraps pipe
*****************************************************************************/
void SegmentAssembler::flush() {

for( set<SwiftPacket*,before>::iterator it= pool.begin();
     it != pool.end(); ++it) {

    _scraps << *it;

}

pool.clear();

} // end of flush method


