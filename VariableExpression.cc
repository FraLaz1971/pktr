#include "VariableExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
VariableExpression::VariableExpression(const string& name) {
    this->name = name;
} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* VariableExpression::eat(istream* in) {


    skipWhiteSpace(in);

    /************************
    * fields start with "$" *
    ************************/
    if(in->peek() != '$' ) return NULL;
    in->get();
    
    string name;
    while(true) {

        char c = in->peek();
        if(isWordChar(c) ) {
            /****************************
            * this is a valid character *
            ****************************/
            name += c;
            in->get();
        } else break;

    
    }

    if(name == "" ) return NULL;       
    else            return new VariableExpression(name);         
}


/*********************************************************************
*
*********************************************************************/
double VariableExpression::evaluate(Context* c) throw(Exception) {

    return c->get(name);

} // end of evaluate method

/*********************************************************************
*
*********************************************************************/
void VariableExpression::set(Context* c, double value) throw(Exception) {

    c->set(name, value);

} // end of evaluate method



