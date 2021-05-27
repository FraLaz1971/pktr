#include "Expression.h"

//#include "VariableExpression.h"
//#include "ConstantExpression.h"
//#include "PlusMinusExpression.h"


#include <sstream>

/*********************************************************************
* constructor
*********************************************************************/
Expression::Expression() {
} // end of constructor

/*********************************************************************
* returns the evaluated value of the child node at a given index.
*********************************************************************/
double Expression::arg(int i, Context* c) {

    return args[i]->evaluate(c);
}

/*********************************************************************
* returns true if the expression has collected all the args it needs
* and is ready to be evaluated
*********************************************************************/
bool Expression::isEvaluable() { 

//    cout << "(calling Expression::isEvaluable)\n";

    return !hasArgs() || args.size() >0; 

}


/*****************************************************************************
* This is the static factory method which should be used to create
* an expression. It returns a pointer to a new Expression which is the root
* node of a tree describing the text string
*****************************************************************************/
Expression* Expression::parse(const string& text) throw(Exception) {


    TokenList tokens;
    tokens.parse(text);
    tokens.collect();

//cerr << "just parsed: "<<tokens.first() << "\n";

    return tokens.first();
}


/***************************************************************************
* printing operator. You can feed a pointer to an expression to an ostream
* to print a human readable representation of the entire tree underneath it.
***************************************************************************/
ostream& operator<<(ostream& out, Expression* e) {

    if(e==NULL) out << "null";
    else        e->print(out);
    return out;
}


/**************************************************************************
* This function is called by the printing operator. Subclasses should override
* this method and not the operator to provide their own representations.
**************************************************************************/
void Expression::print(ostream& out) const{ 
    out << "?"; 
}


/*********************************************************************
* by default assignment is not allowed
*********************************************************************/
void Expression::set(Context* c, double value) throw(Exception) {
    throw Exception("Illegal assignment");
}
