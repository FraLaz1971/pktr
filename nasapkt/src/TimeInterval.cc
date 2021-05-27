#include "TimeInterval.h"

#include <iostream>
#include <iomanip>

/****************************************************************************
* constructor
****************************************************************************/
TimeInterval::TimeInterval(double start, double stop, unsigned int id/*=0*/) 
              throw(Exception){

    set(start,stop,id);

} // end of constructor

/****************************************************************************
* copy constructor
****************************************************************************/
TimeInterval::TimeInterval(TimeInterval& interval) {

    _start = interval.start();
    _stop  = interval.stop();
    _id    = interval.id();

} // end of copy constructor

/****************************************************************************
* set all the values
****************************************************************************/
void TimeInterval::set(double start, double stop, unsigned int id/*=0*/) 
                   throw(Exception) {

    if(stop<= start) throw Exception("Invaid interval times");

    _start = start;
    _stop  = stop;
    _id    = id;

} // end of set method

/****************************************************************************
* set the start time value
****************************************************************************/
void TimeInterval::start(double start) throw(Exception) { 

    if(start >= _stop) throw Exception("Illegal start time");
    _start=start; 
}

/****************************************************************************
* set the start time value
****************************************************************************/
void TimeInterval::stop(double stop) throw(Exception) { 

    if(_start >= stop) throw Exception("Illegal stop time");
    _stop=stop;
}

/****************************************************************************
* returns true if the time interval contains the given time
****************************************************************************/
bool TimeInterval::contains(double time) {

//cout << _start<<" to "<<_stop<<" time="<<time<<"\n";

    return _start <= time && time < _stop;

}

/****************************************************************************
* returns true if the time interval covers any of the same time as this one
* note that the endpoint is not included in the interval, so returns
* false if the intervals just touch each other.
****************************************************************************/
bool TimeInterval::overlaps(TimeInterval* interval) {

    return !touches(interval) && (
               contains(interval->start()) ||
               contains(interval->stop() ) ||
               interval->contains(_start ) ||
               interval->contains(_stop )    
           );
}


/****************************************************************************
* returns true if two intervals overlap or if they have the same ID and
* are just touching one another.
****************************************************************************/
bool TimeInterval::canMergeWith(TimeInterval* interval) {

return overlaps(interval) ||
       ( _id == interval->id() && touches(interval) );

}

/****************************************************************************
* returns true if the time interval covers any of the same time as this one
****************************************************************************/
bool TimeInterval::touches(TimeInterval* interval) {

    return interval->start() == _stop ||
            interval->stop() == _start  ;
}

/****************************************************************************
* add another time interval to this one throws an exception if the
* intervals to not have the same ID or if they do not overlap
****************************************************************************/
void TimeInterval::mergeWith(TimeInterval* interval) throw(Exception) {


    if(_id != interval->id() ) {
        throw Exception("IDs are not the same. Can't merge\n"+ 
                        this->toString()+"\n"+interval->toString());
    }

    if(! canMergeWith(interval) ) {
        throw Exception("Intervals don't overlap. Can't merge\n"+
                        this->toString()+"\n"+interval->toString() );
    }
    
    if(interval == this) {
        throw Exception("Merging "+this->toString()+" with its clone");
    }

    if(interval->start() < _start) _start = interval->start();
    if(interval->stop()  > _stop ) _stop  = interval->stop();

} // end of mergeWith method


/****************************************************************************
* Returns true if the time intervals do not overlap
* and the left interval preceeds the right one.
****************************************************************************/
bool operator<(const TimeInterval& left, const TimeInterval& right) {
    return left.start() < right.start();
}


/****************************************************************************
* Returns true if the given time time comes after the interval
****************************************************************************/
bool operator<(const TimeInterval& left, double        time ){
    return left.stop() < time;
}

/****************************************************************************
* Returns true if the given time time comes before the interval
****************************************************************************/
bool operator<(double       time, const TimeInterval& right){
    return time < right.start();
}



/****************************************************************************
* returns true if two intervals have the same start and stop times and ID tag.
****************************************************************************/
bool operator==(TimeInterval& left, TimeInterval& right){

    return left.start() == right.start() &&
           left.stop()  == right.stop()  &&
           left.id()    == right.id();
}



/****************************************************************************
* printing operator
****************************************************************************/
ostream& operator<<(ostream& out, TimeInterval* interval) {

    int orig = out.precision();

    out << setprecision(15)
        << interval->start() <<" "
        << interval->stop()  <<" "
        << interval->id()
        << setprecision(orig);

    return out;

}


