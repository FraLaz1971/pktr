#ifndef ACSTIME_LINE_UPDATER_H
#define ACSTIME_LINE_UPDATER_H

#include "TimeLineUpdater.h"

/*****************************************************************************
* This is a TimelineUpdater which knows how to read ACS packets.
* it assumes every packet wich passes through it is a valid ACS
* packet of type "SwiftPacket".
*
*****************************************************************************/
class ACSTimeLineUpdater : public TimeLineUpdater {

private:
    /*******
    * data *
    *******/
    ACS acs;


public:
    /**************
    * constructor *
    **************/
    ACSTimeLineUpdater(TimeLine* timeline);

    /*************
    * destructor *
    *************/
    virtual ~ACSTimeLineUpdater() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual bool parsePacket(CCSDSPacket* p);
 
}; // end of ACSTimeLineUpdater class

#endif // ACSTIME_LINE_UPDATER_H
