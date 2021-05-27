#include "SwiftIDCounter.h"

#include "LDPPacket.h"
#include "SwiftHead3Packet.h"


/*********************************************************************
* constructor
*********************************************************************/
SwiftIDCounter::SwiftIDCounter() {
    first=true;

} // end of constructor

/*********************************************************************
*
*********************************************************************/
void SwiftIDCounter::report(unsigned char obs, unsigned int target) {

    if(first || last_obs != obs || last_target != target) {

        cout << "target="<<target<<" obs="<<(int)obs<<"\n";

        last_obs    = obs;
        last_target = target;
        first=false;
    }

}


/*********************************************************************
* This function does the work of the PacketPipe.
*********************************************************************/
CCSDSPacket* SwiftIDCounter::operateOnPacket(CCSDSPacket* generic_p) {

    /******************************************
    * try casting the packet as an LDP packet *
    ******************************************/
    LDPPacket* ldp = dynamic_cast<LDPPacket*>(generic_p);
    if(ldp != NULL) {
        if(ldp->page() != 1) return ldp;
        /*******************************
        * this is an page 1 LDP packet *
        *******************************/
        unsigned char* data = ldp->data() + 2; // skip the 2 byte page count

        unsigned char obs = data[0];

        unsigned int target=0;
        for(int i=1; i<4; ++i) {
            target <<= 8;
            target |= data[i];
        }

        report(obs,target);

        return ldp;
    
    }

    /*******************************************
    * try casting the packet as a HEAD3 packet *
    *******************************************/
    SwiftHead3Packet* head3 = dynamic_cast<SwiftHead3Packet*>(generic_p);
    if(head3 != NULL) {
        /*************************
        * this is a HEAD3 packet *
        *************************/
        unsigned char obs   = head3->obs();
        unsigned int target = head3->target();

        report(obs,target);

        return head3;
    
    }

    /***********************************************
    * any other type of packet is silently ignored *
    ***********************************************/
    return generic_p;

}
