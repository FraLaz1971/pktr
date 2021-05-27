#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <iostream>
using namespace std;

/********************************************************************************
* This class represents a CCSDS sequence counter. It can be used nearly 
* interchangeably with integers. It defines basic operators like subtraction,
* addition and comparison with counter rollover taken into account.
********************************************************************************/
class Sequence {

    /************
    * constants *
    ************/
public:
    static const int PERIOD = 1<<14;
    static const int HALF_PERIOD = PERIOD>>1;

protected:
    /*******
    * data *
    *******/
    int value;


public:
    /**************
    * constructor *
    **************/
    Sequence(int value=0);

    /*************
    * destructor *
    *************/
    virtual ~Sequence() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/

    /***********
    * operators *
    ************/
    friend const int  operator-(const Sequence& left, const Sequence& right);
    friend const int  operator+(const Sequence& left, const Sequence& right);
    friend const bool operator<(const Sequence& left, const Sequence& right);
    friend const bool operator>(const Sequence& left, const Sequence& right);
    friend const bool operator==(const Sequence& left, const Sequence& right);
    friend const bool operator<=(const Sequence& left, const Sequence& right);
    friend const bool operator>=(const Sequence& left, const Sequence& right);

    friend ostream& operator<<(ostream& out, Sequence& interval);


}; // end of Sequence class

#endif // SEQUENCE_H
