#include "UVOTIDSetter.h"

/*********************************************************************
* constructor
*********************************************************************/
UVOTIDSetter::UVOTIDSetter() {
} // end of constructor


/*********************************************************************
* This function does the work of the PacketPipe.
*********************************************************************/
CCSDSPacket* SwiftIDSetter::operateOnPacket(CCSDSPacket* generic_p) {

    /******************************************
    * try casting the packet as a HEAD2 packet *
    ******************************************/
    SwiftPacket* p = dynamic_cast<SwiftPacket*>(generic_p);
    if(p != NULL) {

        if(! p->isFirst() ) return p;




        if(ldp->page() != 1) return ldp;
        /*******************************
        * this is an page 1 LDP packet *
        *******************************/
        unsigned char* data = ldp->data() + 2; // skip the 2 byte page count

        data[0] = obs;
        for(int i=0; i<3; ++i) {
            data[i+1] = target_bytes[i];
        }

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
        head3->obs(obs);
        head3->target(target);

        return head3;
    
    }

    /***********************************************
    * any other type of packet is silently ignored *
    ***********************************************/
    return generic_p;

}

