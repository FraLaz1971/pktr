#include "StringExpression.h"

#include "FieldExpression.h"
#include "VariableExpression.h"

#include <sstream>

/*********************************************************************
* constructor
*********************************************************************/
StringExpression::StringExpression(const string& value) {

    this->value = value;

    istringstream* in = new istringstream(value.c_str());
    string sub="";
    while(in->good()) {

        /********************************************
        * make sure white space doesn't get eaten
        * when trying to parse variable names 
        ********************************************/
        if(Expression::isWhiteSpace(in->peek()) ) {
            sub+=in->get();
            continue;
        }

        Expression* e;
        e = FieldExpression::eat(in);
        if(e) {
            substrings.insert(substrings.end(), sub);
            sub="";
            args.insert(args.end(), e);
            continue;
         }

        e = VariableExpression::eat(in);
        if(e) {
            substrings.insert(substrings.end(), sub);
            sub="";
            args.insert(args.end(), e);
            continue;
         }


         if(in->good() && in->peek()>=0) sub += in->get();
         else break;

    } // end of loop for parsing string

    substrings.insert(substrings.end(), sub);

} // end of constructor

/*********************************************************************
*
*********************************************************************/    
Expression* StringExpression::eat(istream* in) {


    skipWhiteSpace(in);
    if(in->peek() != '"' ) return NULL;


    bool esc=false;
    string value = "";
    in->get();
    while(*in && in->peek() != '"' ) {

        if(in->peek() == '\\' ) {
            /******************************
            * start of an escape sequence *
            ******************************/
            esc=true;
            in->get();
            continue;
        }

        if(esc) {
            /************************************
            * second char of an escape sequence *
            ************************************/
            if(     in->peek() == 'n' ) value += '\n';
            else if(in->peek() == 't' ) value += '\t';
            else value += in->peek();
            in->get();
            esc=false;
        } else {
            /***************************
            * just a regular character *
            ***************************/
            value += in->get();
        }
    }

    /**************************
    * clear the closing quote *
    **************************/
    if(in->peek() == '"' ) in->get();

    return new StringExpression(value);

} // end of eat method


/*********************************************************************
* strings print themselves and then return 0;
*********************************************************************/   
double StringExpression::evaluate(Context* c) throw(Exception) {

    ostream* out = c->out();

    for(int i=0; i<(int)substrings.size(); ++i) {
        
        *out << substrings[i];
        if(i<(int)args.size() ) *out << arg(i,c);

    }

    return 0.0;

} // end of evaluate method


/*********************************************************************
*
*********************************************************************/    
void StringExpression::print(ostream& out) const {

    out << '"'<<value<<'"';
}

