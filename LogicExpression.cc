#include "LogicExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
LogicExpression::LogicExpression(bool is_and) {

    this->is_and = is_and;

    if(is_and) name = "&&";
    else       name = "||";

} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* LogicExpression::eat(istream* in) {

    skipWhiteSpace(in);

    /******
    * and *
    ******/
    if(in->peek() == '&') {
        in->get();
        if(in->peek() == '&' ) {
            in->get();
            return new LogicExpression(true);
        } else {
            in->unget();
            return NULL;
        }
    }

    /*****
    * or *
    *****/
    if(in->peek() == '|') {
        in->get();
        if(in->peek() == '|' ) {
            in->get();
            return new LogicExpression(false);
        } else {
            in->unget();
            return NULL;
        }
    }

    /**********************
    * didn't get anything *
    **********************/
    return NULL;

} // end of eat static method

/*********************************************************************
*
*********************************************************************/
double LogicExpression::evaluate(Context* c) throw(Exception) {

    bool result;
    if(is_and) result = (arg(0,c)!=0) && (arg(1,c)!=0);
    else       result = (arg(0,c)!=0) || (arg(1,c)!=0);

    if(result) return 1.0;
    else       return 0.0;

} // end of evaluate method

