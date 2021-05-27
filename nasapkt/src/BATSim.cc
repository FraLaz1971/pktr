#include "BATSim.h"

#include <fstream>

/*****************************************************************************
* constructor
*****************************************************************************/
BATSim::BATSim() {

writer = new PacketWriter(new ofstream("z"));

size=942;
packet = new LDPPacket(size);
packet_time_interval = .05;

}

/*****************************************************************************
* pretend to observe a target
*****************************************************************************/
void BATSim::observe(double ra, double dec, int target, int obs, 
                     double end_time) {




StandardLDPHead* head = new StandardLDPHead();

head->npages(0);
head->head3()->target(target);
head->head3()->obs(obs);
head->head3()->sc_time()->value(packet->time());

/********************************************
* loop over the duration of the observation *
********************************************/
while(packet->time() < end_time) {

    cout << "product "<<packet->product()<<" time="<<packet->time()<<"\n";

    /***********************************
    * set the observation science time *
    ***********************************/
    head->head3()->sc_time()->value(packet->time());



    /********************************************************
    * create a new LDP encoder, wrap it in a data stream 
    * and hook it to the packet writer
    ********************************************************/
    LDPEncodeBuffer* ldp = new LDPEncodeBuffer(packet, head, NULL, 
                                               packet_time_interval);
    ostream* out = new ostream(ldp);
    ldp->pipe() | *writer;

    /***********************
    * loop over data bytes *
    ***********************/
    for(int i=0; i<10000000 && packet->time() < end_time; ++i) {

        out->put(i%256);
    }

    /*********************
    * finish off the LDP *
    *********************/
    out->flush();
    ldp->close();

    delete ldp;
    delete out;

    /************************************
    * reset the packet for the next LDP *
    ************************************/
    packet->size(size);
    packet->product(packet->product()+1);

} // end of loop over LDPs


} // end of observe method
