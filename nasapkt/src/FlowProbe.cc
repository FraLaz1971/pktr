#include "FlowProbe.h"

/*********************************************************************
* constructor
*********************************************************************/
FlowProbe::FlowProbe() {
} // end of constructor

/*********************************************************************
*
*********************************************************************/
CCSDSPacket* FlowProbe::operateOnPacket(CCSDSPacket* p) {

if(p!=NULL) { 
    cout << "PROBE: "<<p
         <<" APID="<<p->apid()
         << " sequence="<<p->sequence()
         <<"\n";
} else {
    cout << "PROBE: End of Stream\n";
}

return p;
}
