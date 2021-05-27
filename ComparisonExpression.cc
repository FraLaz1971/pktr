#include "ComparisonExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
ComparisonExpression::ComparisonExpression(bool less, bool equals, bool more) {

    this->less   = less;
    this->equals = equals;
    this->more   = more;

    if(less && more ) name = "!=";
    else if(!less && !more ) name = "==";
    else if(less) {
        name = "<";
        if(equals) name += "=";
    } else if(more) {
        name = ">";
        if(equals) name += "=";
    }

} // end of constructor


/*********************************************************************
*
*********************************************************************/
Expression* ComparisonExpression::eat(istream* in) {

    skipWhiteSpace(in);

    if(in->peek() == '!') {
        in->get();
        if(in->peek() == '=' ) {
            in->get();
            return new ComparisonExpression(true, false, true);
        } else {
            in->unget();
            return NULL;
        }
    }

    if(in->peek() == '=') {
        in->get();
        if(in->peek() == '=' ) {
            in->get();
            return new ComparisonExpression(false, true, false);
        } else {
            in->unget();
            return NULL;
        }
    }

    if(in->peek() == '<') {
        in->get();
        if(in->peek() == '=' ) {
            in->get();
            return new ComparisonExpression(true, true, false);
        } else {
            return new ComparisonExpression(true, false, false);
        }
    }

    if(in->peek() == '>') {
        in->get();
        if(in->peek() == '=' ) {
            in->get();
            return new ComparisonExpression(false, true, true);
        } else {
            return new ComparisonExpression(false, false, true);
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
double ComparisonExpression::evaluate(Context* c) throw(Exception) {

    bool result=false;
    if(less  ) result = result || arg(0,c) <  arg(1,c);
    if(equals) result = result || arg(0,c) == arg(1,c);
    if(more  ) result = result || arg(0,c) >  arg(1,c);

    if(result) return 1.0;
    else       return 0.0;

} // end of evaluate method

