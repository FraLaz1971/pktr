#include "ACSTimeLineUpdater.h"

/*********************************************************************
* constructor
*********************************************************************/
ACSTimeLineUpdater::ACSTimeLineUpdater(TimeLine* timeline) 
                   : TimeLineUpdater(timeline) {

} // end of constructor

/*********************************************************************
* returns true if this is a valid packet
*********************************************************************/
bool ACSTimeLineUpdater:: parsePacket(CCSDSPacket* p) {

    Reader* r = p->dataReader();
    acs.read(r);
    delete r;

    id = (acs.target() << 8) + acs.obs();
    time = acs.time()->value();

    margin = 0.01;

    return true;
    

} // end of parsePacket method
