#include "TimeFilter.h"

#include "SwiftPacket.h"

/*********************************************************************
* constructor
*********************************************************************/
TimeFilter::TimeFilter(double min_time, double max_time) {

    this->min_time = min_time;
    this->max_time = max_time;

} // end of constructor


/*********************************************************************
*
*********************************************************************/
bool TimeFilter::accept(CCSDSPacket* p) {

    if(p==NULL) return true;
    
    SwiftPacket* cast = dynamic_cast<SwiftPacket*>(p);
    if(cast==NULL) return false;
    
    
    double time = cast->bestTime();
    
    return time >= min_time && time < max_time;
    
} // end of accept method

