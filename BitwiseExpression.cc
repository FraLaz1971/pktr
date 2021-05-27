#include "BitwiseExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
BitwiseExpression::BitwiseExpression(bool is_and) {

     this->is_and = is_and;

} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* BitwiseExpression::eat(istream* in) {

    skipWhiteSpace(in);

    if(in->peek() == '&') {
        in->get();
        if(in->peek() != '&' ) {
            in->get();
            return new BitwiseExpression(true);
        } else {
            in->unget();
            return NULL;
        }
    }

    if(in->peek() == '|') {
        in->get();
        if(in->peek() != '|' ) {
            in->get();
            return new BitwiseExpression(false);
        } else {
            in->unget();
            return NULL;
        }
    }

    return NULL;

} // end of eat static method


/*********************************************************************
*
*********************************************************************/
double BitwiseExpression::evaluate(Context* c) throw(Exception) {

    if(is_and) return (double)((long)arg(0,c) & (long)arg(1,c));
    else       return (double)((long)arg(0,c) | (long)arg(1,c));

} // end of evaluate method
