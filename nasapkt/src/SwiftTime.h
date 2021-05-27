#ifndef SWIFT_TIME_H
#define SWIFT_TIME_H

#include "SwiftTimeBlock.h"

/*****************************************************************************
* This class extends the machine-generated SwiftTimeBlock class to allow
* direct access to the Swift-encoded time value as a real number.
* This class should almost always be used instead of the less functional
* SwiftTimeBlock class.
*****************************************************************************/
class SwiftTime : public SwiftTimeBlock {

private:

    constexpr static const double resolution=2e-5; // 20 microseconds per subsecond tick

public:
    /************
    * accessors *
    ************/
    virtual double value();
    virtual void value(double time);


    friend int operator==(SwiftTime& left,
                          SwiftTime& right);

    friend int operator!=(SwiftTime& left, 
                          SwiftTime& right);


}; // end of SwiftTime class

#endif // SWIFT_TIME_H
