#include "PlusMinusExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
PlusMinusExpression::PlusMinusExpression(bool positive) {

    this->positive = positive;

    if(positive) name = "+";
    else         name = "-";

} // end of constructor

/*********************************************************************
* depends on whether this is unary or binary
*********************************************************************/
int PlusMinusExpression::precedence() { 

    if(unary) return 1;
    else      return 4;
}



/*********************************************************************
*
*********************************************************************/
Expression* PlusMinusExpression::eat(istream* in) {

    skipWhiteSpace(in);

    if(in->peek() == '-') {
        /*********************
        * found a minus sign *
        *********************/
        in->get();
        return new PlusMinusExpression(false);
    }

    if(in->peek() == '+') {
        /*********************
        * found a plus sign *
        *********************/
        in->get();
        return new PlusMinusExpression(true);
    }


    return NULL;

} // end of eat static method

/*********************************************************************
* Determine whether this is a unary +/- or binary
* we are unary if there is nothing or an operator on the left.
*********************************************************************/
void PlusMinusExpression::firstPass(TokenList* tokens) {
    
    Expression* left = tokens->get(-1);
    unary = left==NULL || dynamic_cast<OperatorExpression*>(left);

} // end of firstPass method


/*********************************************************************
* 
*********************************************************************/
void PlusMinusExpression::collectArgs(TokenList* tokens) {

    if(unary) collectUnaryArgs(tokens);
    else      collectBinaryArgs(tokens);

} // end of collectArgs method


/*********************************************************************
*
*********************************************************************/
double PlusMinusExpression::evaluate(Context* c) throw(Exception) {

    if(unary) {
        /********
        * unary *
        ********/
        if(positive) return  arg(0,c);
        else         return -arg(0,c);
    } else {
        /*********
        * binary *
        *********/
        if(positive) return  arg(0,c) + arg(1,c);
        else         return  arg(0,c) - arg(1,c);
    }


} // end of evaluate method
