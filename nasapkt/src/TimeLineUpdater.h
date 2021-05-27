#ifndef TIME_LINE_UPDATER_H
#define TIME_LINE_UPDATER_H

#include "PacketPipe.h"

#include "TimeLine.h"
#include "ACS.h"

/*****************************************************************************
* This is a PacketPipe which adds an interval to a timeline for each
* packet which passes through it. The packets themselves are unchanged
* as they go through the pipe.
*
* This is actually an abstract superclass, which doesn't know how to parse
* packets. This functionality is added by subclasses.
*
*****************************************************************************/
class TimeLineUpdater : public PacketPipe {

private:
    /*******
    * data *
    *******/
    TimeLine* timeline;
    int errors;

protected:
    double time;
    double margin;
    unsigned int id;


public:
    /**************
    * constructor *
    **************/
    TimeLineUpdater(TimeLine* timeline);

    /*************
    * destructor *
    *************/
    virtual ~TimeLineUpdater() {}

    /************
    * accessors *
    ************/
    virtual int getErrorCount();

    /**********
    * methods *
    **********/
    virtual CCSDSPacket* operateOnPacket(CCSDSPacket* p);

    virtual bool parsePacket(CCSDSPacket* p) =0;



}; // end of TimeLineUpdater class

#endif // TIME_LINE_UPDATER_H
