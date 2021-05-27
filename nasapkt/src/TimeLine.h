#ifndef TIMELINE_H
#define TIMELINE_H

#include <cstdlib>
#include <iostream>
#include <set>
#include "TimeInterval.h"

/****************************************************************************
* This class represents a set of TimeInterval objects. It is used by
* the TimePacketBinner class to sort packets.
****************************************************************************/
class TimeLine {

private:
    /************************************************
    * embedded comparison class for sorting the set *
    ************************************************/
    class lessThan {
        public:
        bool operator()(const TimeInterval* left, const TimeInterval* right) {
            return *left < *right;
        }
    }; // end of embedded class for comparisons

private:
    /*******
    * data *
    *******/
    multiset<TimeInterval*, lessThan> s;
    multiset<TimeInterval*, lessThan>::iterator add_position;
    multiset<TimeInterval*, lessThan>::iterator search_position;

public:
    /**************
    * constructor *
    **************/
    TimeLine() { 
        search_position = add_position = s.end();
    };

    /**************
    * destructor *
    **************/
    virtual ~TimeLine() {}


    /**********
    * methods *
    **********/
    virtual void addInterval(TimeInterval* interval)
                 throw(TimeInterval::Exception);
                 
    virtual void removeInterval(TimeInterval* interval)
                 throw(TimeInterval::Exception);
                 
    virtual void removeIntervalWhereConsistent(TimeInterval* interval)
                 throw(TimeInterval::Exception);

    virtual void forceInterval(TimeInterval* interval) throw(TimeInterval::Exception);

    virtual void mergeWith(TimeLine* timeline)
                 throw(TimeInterval::Exception);

    virtual void overlay(TimeLine* timeline) throw(TimeInterval::Exception);

    virtual void subtract(TimeLine* timeline) throw(TimeInterval::Exception);

    virtual void leaveInconsistencies(TimeLine* timeline) 
                 throw(TimeInterval::Exception);
                 
    virtual int getIntervalCount() { return s.size(); }
    virtual TimeInterval* getInterval(int index);


    virtual TimeInterval* findInterval(double time);
    virtual TimeLine* findIntervals(double start, double stop);
    virtual void fillGaps(double allowed_gap);
    virtual void splitGaps(double allowed_gap, double latest_allowed_time);

    virtual void read( istream* in ) throw(TimeInterval::Exception);
    virtual void write(ostream* out);

    void print() {
        for(multiset<TimeInterval*, lessThan>::iterator it = s.begin();
            it != s.end(); ++it ) {
            cout << (*it)->start() <<" "<< (*it)->stop() << "\n";
        }
    }



}; // end of TimeLine class
#endif // TIMELINE_H
