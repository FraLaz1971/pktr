#ifndef COMPARISON_EXPRESSION_H
#define COMPARISON_EXPRESSION_H

#include "OperatorExpression.h"

class ComparisonExpression : public OperatorExpression {

private:
    /*******
    * data *
    *******/
    bool less;
    bool equals;
    bool more;


public:
    /**************
    * constructor *
    **************/
    ComparisonExpression(bool less, bool equals, bool more);

    /*************
    * destructor *
    *************/
    virtual ~ComparisonExpression() {}

    /************
    * accessors *
    ************/
    virtual int precedence() { return 5; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);


}; // end of ComparisonExpression class

#endif // COMPARISON_EXPRESSION_H
