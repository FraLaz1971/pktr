#ifndef TIME_FILTER_H
#define TIME_FILTER_H

#include "PacketFilter.h"

/*******************************************************************************
*
*******************************************************************************/
class TimeFilter : public PacketFilter {

private:
    /*******
    * data *
    *******/
    double min_time;
    double max_time;


public:
    /**************
    * constructor *
    **************/
    TimeFilter(double min_time, double max_time);

    /*************
    * destructor *
    *************/
    virtual ~TimeFilter() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    bool accept(CCSDSPacket* p);

}; // end of TimeFilter class

#endif // TIME_FILTER_H
