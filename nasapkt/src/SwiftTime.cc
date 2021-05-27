#include "SwiftTime.h"

#include <iomanip>

/****************************************************************************
* return the decoded time value
****************************************************************************/
double SwiftTime::value() { 


// Note when I did "return seconds() + resolution * subseconds();"
// under Intel Linux I got random roundoff errors which cause
// time comparisons to not work. putting the value in a variable
// before returning seemes to have fixed the problem

double val = seconds() + resolution * subseconds();

/*
cout << "seconds="<<seconds()<<" subseconds="<<subseconds()
     << " value=" << setprecision(20) <<val<<"\n";
*/

return val;
} // end of value method

/****************************************************************************
* Encode the time value. Times will be rounded to the nearest integral
* subsecond.
****************************************************************************/
void SwiftTime::value(double time) {

    unsigned int sec = (int) time;
    unsigned int subsec = (int)((time - (double)sec)/resolution + 0.5);

    seconds(sec);
    subseconds(subsec);

} // end of set time method


int operator==(SwiftTime& left, SwiftTime& right) { 
    return left.value() == right.value();
}

int operator!=(SwiftTime& left, SwiftTime& right) {
    return left.value() != right.value();

}
