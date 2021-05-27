#include "NotExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
NotExpression::NotExpression() {

    name = "!";

} // end of constructor


/*********************************************************************
*
*********************************************************************/
Expression* NotExpression::eat(istream* in) {

    skipWhiteSpace(in);

    if(in->peek() == '!') {
        /***********
        * division *
        ***********/
        in->get();
        if(in->peek() != '=' ) {
            return new NotExpression();
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
double NotExpression::evaluate(Context* c) throw(Exception) {

    if(arg(0,c)==0) return 1.0;
    else            return 0.0;

} // end of evaluate method


/*********************************************************************
* unary
*********************************************************************/
void NotExpression::collectArgs(TokenList* tokens) {
    collectUnaryArgs(tokens);
}
