#ifndef APIDFILTER_H
#define APIDFILTER_H

#include "PacketFilter.h"
#include <vector>

class APIDFilter : public PacketFilter {

private:
    /***********************************************************
    * inner class to hold an APID interval
    ***********************************************************/
    class Interval {
        private:
        int from;
        int to;

        public:
        Interval(int from, int to) { 
            this->from = from;
            this->to   = to;
        }

        bool contains(int apid) {
            return apid>= from && apid <= to; 
        }
    }; // end of inner class Interval
        

private:
    /*******
    * data *
    *******/
    vector<Interval*> intervals;


public:
    /**************
    * constructor *
    **************/
    APIDFilter();

    /*************
    * destructor *
    *************/
    virtual ~APIDFilter();

    /************
    * accessors *
    ************/
    virtual void addInterval(int from, int to);
    virtual void addIntervals(const string& list);

    /**********
    * methods *
    **********/
    virtual bool accept(CCSDSPacket* p);

}; // end of APIDFilter class

#endif // APIDFILTER_H
