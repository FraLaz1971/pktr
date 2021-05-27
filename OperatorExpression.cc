#include "OperatorExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
OperatorExpression::OperatorExpression() {
} // end of constructor

/*********************************************************************
*
*********************************************************************/
void OperatorExpression::print(ostream& out) const {

    int nargs = args.size();
    if(nargs == 0 ) out << name;
    else if(nargs==1) out << name << args[0];
    else if(nargs==2) out << "(" << args[0] << name << args[1] << ")";

}


/*********************************************************************
* This is a generic routine for binary operators
*********************************************************************/
void OperatorExpression::collectBinaryArgs(TokenList* tokens) {

    if(this != tokens->get(0) ) {
        cerr << "iterator out of place\n";
        return;
    }

    /*****************************************
    * skip this if we already have arguments *
    *****************************************/
    if(isEvaluable() ) return;

//    cout << "    collecting binary args for "<<this<<"\n";

    /***************************************
    * get the tokens on the left and right *
    ***************************************/
    Expression*  two_left = tokens->get(-2);
    Expression*      left = tokens->get(-1);
    Expression*     right = tokens->get( 1);
    Expression* two_right = tokens->get( 2);

    if((left && left->isEvaluable() ) &&
       (!two_left || !two_left->hasRightArg() ||
        two_left->precedence() > precedence()   ) &&

       (right && right->isEvaluable() ) &&
       (!two_right || !two_right->hasLeftArg() ||
        two_right->precedence() >= precedence()   ) ) {
        /**************************************************
        * the arguments on either side are ready to go
        * and nobody else with higher priority wants them
        **************************************************/
        args.insert(args.end(), left);
        args.insert(args.end(), right);
        tokens->remove(1);
        tokens->remove(-1);
    }


} // end of collectBinaryArgs method

/*********************************************************************
* This is a generic routine for binary operators
*********************************************************************/
void OperatorExpression::collectUnaryArgs(TokenList* tokens) {

    if(this != tokens->get(0) ) {
        cerr << "iterator out of place\n";
        return;
    }

    /*****************************************
    * skip this if we already have arguments *
    *****************************************/
    if(isEvaluable() ) return;

//    cout << "    collecting unary args for "<<this<<"\n";

    /*****************************
    * get the token on the right *
    *****************************/
    Expression*     right  = tokens->get(1);
    Expression* two_right  = tokens->get(2);


    if(right  &&  right->isEvaluable() &&

       (!two_right || !two_right->hasLeftArg() ||
        two_right->precedence() >= precedence()   ) ) {
        /***********************************************
        * the arguments on either side are ready to go *
        ***********************************************/
        args.insert(args.end(), right);
        tokens->remove(1);
    }


} // end of collectUnaryArgs method



/*********************************************************************
* operators are binary by default
*********************************************************************/
void OperatorExpression::collectArgs(TokenList* tokens) {
    collectBinaryArgs(tokens);
}
