#include "ConstantExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
ConstantExpression::ConstantExpression(double value) {
    this->value=value;
} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* ConstantExpression::eat(istream* in) {

    double value;
    *in >> value;
    if(in->fail()) {
        /********************************
        * did not read a valid constant *
        ********************************/
        if(! in->eof() ) in->clear();
        return NULL;
    }

    return new ConstantExpression(value);
}
