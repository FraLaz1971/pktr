#include "LDPPacketList.h"

/*********************************************************************
* constructor
*********************************************************************/
LDPPacketList::LDPPacketList() {

    position = s.end();

} // end of constructor

/*********************************************************************
*
*********************************************************************/
LDPPacketList::addPacket(double time, int sequence, int product) {

    /******************************
    * add the packet to the set *
    ******************************/
    LDPPacketRun* run =  new LDPPacketRun(time sequence, product);

    add_position = s.insert(add_position, run);
    
    /*******************************************************
    * now see if the packet we added belongs to either
    * of the adjacent LDPs
    *******************************************************/


}