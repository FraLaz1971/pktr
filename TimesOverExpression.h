#ifndef TIMES_OVER_EXPRESSION_H
#define TIMES_OVER_EXPRESSION_H

#include "OperatorExpression.h"

class TimesOverExpression : public OperatorExpression {

private:
    /*******
    * data *
    *******/
    bool times;


public:
    /**************
    * constructor *
    **************/
    TimesOverExpression(bool times);

    /*************
    * destructor *
    *************/
    virtual ~TimesOverExpression() {}

    /************
    * accessors *
    ************/
    virtual int precedence() { return 3; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);


}; // end of TimesOverExpression class

#endif // TIMES_OVER_EXPRESSION_H
