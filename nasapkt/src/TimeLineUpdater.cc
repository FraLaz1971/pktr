#include "TimeLineUpdater.h"

#include <iomanip>

/*********************************************************************
* constructor
*********************************************************************/
TimeLineUpdater::TimeLineUpdater(TimeLine* timeline) {

    this->timeline = timeline;
    errors=0;

} // end of constructor

/*********************************************************************
*
*********************************************************************/
int TimeLineUpdater::getErrorCount() { return errors; }

/*********************************************************************
*
*********************************************************************/
CCSDSPacket* TimeLineUpdater::operateOnPacket(CCSDSPacket* p) {

    /************************************************
    * parse the time, margin and id from the packet 
    * unless we are at end of stream
    ************************************************/
    if(p==NULL || !parsePacket(p)) return p;

    try {
        /*****************************************************************
        * try to add an interval centered on this packet to the timeline *
        *****************************************************************/
        timeline->addInterval(new TimeInterval(time-margin, time+margin, id) );

    } catch(TimeInterval::Exception& e) {

        /*********************************************************
        * the interval we tried to add is inconsistent with the
        * existing timeline
        *********************************************************/
        ++errors;

        cerr <<"Ignoring interval: "
             << setprecision(15)
             <<time-margin<<" - "<<time+margin<<" ID="<<id
             <<" : "<<e.what()
             <<"\n";
    }

    /***********************************
    * pass the packet through the pipe *
    ***********************************/
    return p;


} // end of operateOnPacket method
