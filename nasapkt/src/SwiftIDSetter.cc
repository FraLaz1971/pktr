#include "SwiftIDSetter.h"

#include "LDPPacket.h"
#include "SwiftHead3Packet.h"



/*********************************************************************
* constructor
*********************************************************************/
SwiftIDSetter::SwiftIDSetter() {

    obs=0;
    target=0;
    for(int i=0; i<3; ++i) target_bytes[i]=0;

} // end of constructor

/*********************************************************************
* Set the 8 bit observation ID
*********************************************************************/
void SwiftIDSetter::setObs(unsigned char obs) {

    this->obs = obs;
}

/*********************************************************************
* Set the 24 bit target ID
*********************************************************************/
void SwiftIDSetter::setTarget(unsigned int target) {

    this->target=target;

    for(int i=2; i>=0; --i) {
        target_bytes[i] = (target & 0xff);
        target >>= 8;
    }

}


/*********************************************************************
* Get the 8 bit observation ID
*********************************************************************/
unsigned char SwiftIDSetter::getObs() {

    return obs;
}

/*********************************************************************
* Get the 24 bit target ID
*********************************************************************/
unsigned int SwiftIDSetter::getTarget() {

    return target;
}

/*********************************************************************
* This function does the work of the PacketPipe.
*********************************************************************/
CCSDSPacket* SwiftIDSetter::operateOnPacket(CCSDSPacket* generic_p) {

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



