#ifndef LOGIC_EXPRESSION_H
#define LOGIC_EXPRESSION_H

#include "OperatorExpression.h"

class LogicExpression : public OperatorExpression {

private:
    /*******
    * data *
    *******/
    bool is_and;


public:
    /**************
    * constructor *
    **************/
    LogicExpression(bool is_and);

    /*************
    * destructor *
    *************/
    virtual ~LogicExpression() {}

    /************
    * accessors *
    ************/
    virtual int precedence() { return 6; }


    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);


}; // end of LogicExpression class

#endif // LOGIC_EXPRESSION_H
