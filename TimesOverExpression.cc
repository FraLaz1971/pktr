#include "TimesOverExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
TimesOverExpression::TimesOverExpression(bool times) {

    this->times = times;
    
    if(times) name = "*";
    else      name = "/";

} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* TimesOverExpression::eat(istream* in) {

    skipWhiteSpace(in);

    if(in->peek() == '/') {
        /***********
        * division *
        ***********/
        in->get();
        return new TimesOverExpression(false);
    }

    if(in->peek() == '*') {
        /*********************
        * multiplication *
        *********************/
        in->get();
        return new TimesOverExpression(true);
    }

    return NULL;

} // end of eat static method


/*********************************************************************
*
*********************************************************************/
double TimesOverExpression::evaluate(Context* c) throw(Exception) {

    if(times) return  arg(0,c) * arg(1,c);
    else      return  arg(0,c) / arg(1,c);

} // end of evaluate method



