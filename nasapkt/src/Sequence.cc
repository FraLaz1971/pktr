#include "Sequence.h"

/*********************************************************************
* constructor
*********************************************************************/
Sequence::Sequence(int value) {

    this->value = value % PERIOD;
    
} // end of constructor

/*********************************************************************
*
*********************************************************************/
const int operator-(const Sequence& left, const Sequence& right) {

    int diff = left.value - right.value;
    if(diff < -Sequence::HALF_PERIOD) diff += Sequence::PERIOD;
    if(diff >  Sequence::HALF_PERIOD) diff -= Sequence::PERIOD;

    return diff;
}

/*********************************************************************
*
*********************************************************************/
const int operator+(const Sequence& left, const Sequence& right) {

    return left.value + right.value;
}

/*********************************************************************
*
*********************************************************************/
const bool operator<(const Sequence& left, const Sequence& right) {

    return left - right < 0;
}

/*********************************************************************
*
*********************************************************************/
const bool operator>(const Sequence& left, const Sequence& right) {

    return right < left;
}


/*********************************************************************
*
*********************************************************************/
const bool operator==(const Sequence& left, const Sequence& right) {

    return left.value == right.value;
}

/*********************************************************************
*
*********************************************************************/
const bool operator<=(const Sequence& left, const Sequence& right) {

    return left < right || left == right;
}

/*********************************************************************
*
*********************************************************************/
const bool operator>=(const Sequence& left, const Sequence& right) {

    return left > right || left == right;
}

/****************************************************************************
* printing operator
****************************************************************************/
ostream& operator<<(ostream& out, Sequence& seq) {

    out << seq.value;
    
    return out;
}

