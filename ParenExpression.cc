#include "ParenExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
ParenExpression::ParenExpression(bool left) {

    this->left = left;

} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* ParenExpression::eat(istream* in) {

    skipWhiteSpace(in);

    if(in->peek() == '(') {
        in->get();
        return new ParenExpression(true);
    }

    if(in->peek() == ')') {
        in->get();
        return new ParenExpression(false);
    }

    return NULL;

} // end of eat method


/*********************************************************************
*
*********************************************************************/
double ParenExpression::evaluate(Context* c) throw(Exception) {
    throw Exception("Can't evaluate parens");
}


/*********************************************************************
*
*********************************************************************/
void ParenExpression::collectArgs(TokenList* tokens) {

    if(!left) return;

    if(this != tokens->get(0) ) {
        cerr << "iterator out of place\n";
        return;
    }


    Expression* right = tokens->get(1);
    ParenExpression* closing = dynamic_cast<ParenExpression*>(tokens->get(2));

    if(closing && !closing->left && right && right->isEvaluable() ) {
        /****************************************************
        * we have a pair of parens enclosing a single value *
        ****************************************************/
        tokens->remove(2);
        tokens->remove(0);
    }


} // end of collectArgs method


/*********************************************************************
*
*********************************************************************/
void ParenExpression::print(ostream& out) const {

    if(left) out << "[";
    else     out << "]";
}
