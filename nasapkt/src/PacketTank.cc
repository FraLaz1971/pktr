#include <algorithm>

#include "PacketTank.h"

/***************************************************************************
* these are methods for comparing pointers for sorting and uniquing
* Useful if you have a STL container full of pointers to packets
***************************************************************************/
int comes_beforel(CCSDSPacket* left, CCSDSPacket* right) {

    return (*left < *right);

}  // end of comes before function to be used for sorting


/*****************************************************************************
* constructor
*****************************************************************************/
PacketTank::PacketTank() : PacketPipe() {

tank = new deque<CCSDSPacket*>();

unique_packets=0;

} // end of constructor


/*****************************************************************************
* returns the hints flag for the output packet stream
*****************************************************************************/
int PacketTank::hints() {
    int flags = PacketPipe::hints();
    flags |= PacketPipe::ALL_UNIQUE; // output is always unique
    return flags;
}

/*****************************************************************************
* set the hints flag for the incomming packet stream
*****************************************************************************/
void PacketTank::hints(int flags) {

if(flags & PacketPipe::ALL_UNIQUE) unique_packets=1;
else                               unique_packets=0;

PacketPipe::hints(flags);

} // end of set hints method


/*****************************************************************************
* Stable sort the packets into their natural order.
* This method is just a way of simplifying the underlying STL syntax
*****************************************************************************/
void PacketTank::sort() {

    stable_sort(tank->begin(), tank->end(), comes_beforel);

} // end of sort method

/************************************************************************
* delete all but the last of every string of "similar" packets, where
* "similar" means the "unique_comparator" function returns true.
* Note that this is the opposite of normal "uniquing" where the first
* element is usually kept.
* If a pointer to a pipe is given, then all deleted packets will be
* sent down that pipe.
************************************************************************/
void PacketTank::keepLastUnique(PacketPipe* drain /*=NULL*/) {

    /************************************************************
    * create a new container to hold a copy of the data
    * I've tried doing this in place like a million times
    * and each time been tripped up by strange undocumented
    * behavior of iterators when you delete elements from the
    * container.
    * The resulting waste of memory is not as bad as you might expect
    * since the containers hold pointers and not the data thenselves.
    *************************************************************/
    deque<CCSDSPacket*> copy;

    /*******************************************************
    * loop over all the packets
    *******************************************************/
    for(deque<CCSDSPacket*>::iterator it = tank->begin();
        it != tank->end();
        ++it) {

	/******************************************************
	* compare this packet to the one which comes after it *
	******************************************************/
	if(it+1 != tank->end() && unique_comparator(*(it), *(it+1)) ){
	    /******************************************************
	    * the next packet is the same as this one so flush it *
	    ******************************************************/
            if(drain != NULL) *drain << *(it);
	} else {
	    /*************************************************
	    * the next packet is different, so keep this one *
	    *************************************************/
	    copy.insert(copy.end(), *it );
	}
    } // end of loop over packets

    /************************************************
    * now copy the data back into the original tank *
    * I'm sure there's a canned routine for this, but
    * at this point I trust nothing.
    ************************************************/
    tank->clear();
    for(deque<CCSDSPacket*>::iterator it = copy.begin();
        it != copy.end();
        ++it) {

        tank->insert(tank->end(), *it);
    }

 } // end of keepLastUnique method

#ifdef OMIT

/*
This way of doing it didn't work since the begin and end iterators
get all messed up when you delete elements from a container
*/
/*****************************************************************************
* delete all but the last of every string of "similar" packets, where
* "similar" means the "unique_comparator" function returns true.
* Note that this is the opposite of normal "uniquing" where the first
* element is usually kept.
* If a pointer to a pipe is given, then all deleted packets will be
* sent down that pipe.
*****************************************************************************/
void PacketTank::keepLastUnique(PacketPipe* drain /*=NULL*/) {

//cout << "keep last unique" << endl;

    /********************************************************
    * we do this by looping backwards over the list
    * that's much simpler than trying to do it forwards
    ********************************************************/
    for(deque<CCSDSPacket*>::iterator it = tank->end()-1;
        it != tank->begin();
        --it) {


//cout << "comparing "<<(*it) << " and "<<*(it-1)<<endl;

        /*****************************************************************
        * swallow up all adjacent packets which are like this one
        * Note that the adjacent packet is different in each iteration
        * of this loop, since we delete a packet in the body of the loop
        *****************************************************************/
        deque<CCSDSPacket*>::iterator adjacent = it-1;

        while(true) {

            /************************************
            * check if the packets are the same *
            ************************************/
            if(! unique_comparator(*it, *adjacent) ) {
                ++adjacent;
                break;
            }

            /******************************************
            * if we get here the packets are the same *
            ******************************************/
            if(drain != NULL) *drain << *(adjacent);

            /***************************************************
            * we need to stop if we have reached the beginning *
            ***************************************************/
            if(adjacent==tank->begin() ) {
                break;
            }

            --adjacent;
        } 

        if(adjacent != it ) tank->erase(adjacent, it);

 
        /****************************************************************
            * the current packet could become the first packet in the list
            * if we have deleted all the packets between the current one
            * and the start of the list. We need to catch this so that
            * we don't run off the end of the list.
            ***************************************************************/
            if(it == tank->begin() ) return;


         
    } // end of loop over packets 


} // end of  keepLastUnique method



#endif /* OMIT */


/*****************************************************************************
* put a packet into the tank, specifically insert it at the end of the 
* container
*****************************************************************************/
void PacketTank::pushIn(CCSDSPacket* p) {

    /*******************************
    * do nothing for end of stream *
    *******************************/
    if(p==NULL) return;

    /**************************************************
    * make a copy of the packet if the input stream
    * does not all contain unique pointers
    * otherwise use the input packet verbatim
    **************************************************/
    CCSDSPacket* p_copy = NULL;
    if(unique_packets) p_copy = p; 
    else               p_copy = p->copy();   
        
    /****************************************
    * put the packet on the top of the tank *
    ****************************************/
    tank->insert(tank->end(), p_copy );

} // end of pushIn method

/*****************************************************************************
* pull a packet out of the "bottom" of the tank. 
* if the tank is empty, then just pull a packet from upstream,
* like a plain PacketPipe
*****************************************************************************/
CCSDSPacket* PacketTank::pullOut() {

//cout << "tank holds "<<tank->size() <<" packets\n";

if(tank->size() >0) {
    /********************************
    * take a packet out of the tank *
    ********************************/
    CCSDSPacket* p = (*tank)[0];
    tank->pop_front();
    return p;

} else {
    /*****************************************************************
    * take a packet from upstream
    * we put this packet in the tank and then recall this method
    * in case pulling from upstream causes packets to get pushed in.
    * That can happen e.g. with a SwiftTyped packet reader with its
    * drains all tee-ed together.
    *****************************************************************/
    CCSDSPacket* p = PacketPipe::pullOut();
    if(p==NULL && tank->size() == 0 ) {
        return NULL;
    }

    pushIn(p);
    return pullOut();

}

} // end of pullOut method

/*****************************************************************************
* This method overrides the generic PacketPipe method to do nothing.
*****************************************************************************/
void PacketTank::pump() {
} // end of pump method
