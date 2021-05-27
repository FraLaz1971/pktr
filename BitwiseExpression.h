#ifndef BITWISE_EXPRESSION_H
#define BITWISE_EXPRESSION_H

#include "OperatorExpression.h"

class BitwiseExpression : public OperatorExpression {

private:
    /*******
    * data *
    *******/
    bool is_and;


public:
    /**************
    * constructor *
    **************/
    BitwiseExpression(bool is_and);

    /*************
    * destructor *
    *************/
    virtual ~BitwiseExpression() {}

    /************
    * accessors *
    ************/
    virtual int precedence() { return 3; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);


}; // end of BitwiseExpression class

#endif // BITWISE_EXPRESSION_H
