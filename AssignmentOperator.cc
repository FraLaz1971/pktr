#include "AssignmentOperator.h"

/*********************************************************************
* constructor
*********************************************************************/
AssignmentOperator::AssignmentOperator() {
    name="=";
} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* AssignmentOperator::eat(istream* in) {

    skipWhiteSpace(in);

    if(in->peek() == '=' ) {
        in->get();
        if(in->peek() == '=' ) {
            in->unget();
            return NULL;
        }
        return new AssignmentOperator();
    }

    return NULL;

} // end of eat method


/*********************************************************************
*
*********************************************************************/
double AssignmentOperator::evaluate(Context* c) throw(Exception) {

    Expression* left = args[0];

    double value = arg(1,c);
    left->set(c, value);

    return value;

} // end of evaluate
