#ifndef TIME_INTERVAL_H
#define TIME_INTERVAL_H

#include <stdexcept>
#include <sstream>
using namespace std;


/*******************************************************************************
* This class represents a single time interval tagged with an integer ID value.
*******************************************************************************/
class TimeInterval {

public:
    /***************************
    * embedded exception class *
    ***************************/
    class Exception : public runtime_error {

        public:
            Exception(const string& context) : runtime_error(context) { }

    }; // end of Exception class



private:
    /*******
    * data *
    *******/
    double _start;
    double _stop;
    unsigned int _id;

public:
    /**************
    * constructor *
    **************/
    TimeInterval(double start, double stop, unsigned int id=0) throw(Exception);
    TimeInterval(TimeInterval& interval); 

    /************
    * accessors *
    ************/
    virtual void set(double start, double stop, unsigned int id=0) 
                 throw(Exception);

    virtual void start(double start) throw(Exception); 
    virtual void  stop(double stop ) throw(Exception);
    virtual void    id(int    id   ) { _id=id; }

    virtual double start() const { return _start; }
    virtual double  stop() const { return _stop;  }
    virtual unsigned int id() const { return _id;    }

    /**********
    * methods *
    **********/
    virtual bool contains(double time);
    virtual bool overlaps(TimeInterval* interval);
    virtual bool touches(TimeInterval* interval);
    virtual bool canMergeWith(TimeInterval* interval);

    virtual void mergeWith(TimeInterval* interval) throw(Exception);

    virtual string toString() {
        ostringstream out;
        out << this;
#       ifdef FAKING_STRINGSTREAM
	out << ends;
#       endif
        string result = out.str();

        return result;
    }

    /************
    * operators *
    ************/
    friend bool operator<(const TimeInterval& left, const TimeInterval& right);
    friend bool operator<(const TimeInterval& left, double        time );
    friend bool operator<(double        time, const TimeInterval& right);

    friend bool operator==(TimeInterval& left, TimeInterval& right);

    friend ostream& operator<<(ostream& out, TimeInterval* interval);




}; // end of Timeline class
#endif // TIME_INTERVAL_H
