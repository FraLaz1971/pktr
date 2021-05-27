#ifndef STRING_EXPRESSION_H
#define STRING_EXPRESSION_H

#include "Expression.h"

class StringExpression : public Expression {

private:
    /*******
    * data *
    *******/
    string value;
    vector<string> substrings;

    

public:
    /**************
    * constructor *
    **************/
    StringExpression(const string& value);

    /*************
    * destructor *
    *************/
    virtual ~StringExpression() {}

    /************
    * accessors *
    ************/
    virtual bool isEvaluable() { return true; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);

    virtual double evaluate(Context* context) throw(Exception);

    virtual void print(ostream& out) const;



}; // end of StringExpression class

#endif // STRING_EXPRESSION_H
