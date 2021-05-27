#ifndef NOT_EXPRESSION_H
#define NOT_EXPRESSION_H

#include "OperatorExpression.h"

class NotExpression : public OperatorExpression {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    NotExpression();

    /*************
    * destructor *
    *************/
    virtual ~NotExpression() {}

    /************
    * accessors *
    ************/
    virtual int precedence() { return 1; }
    virtual bool hasLeftArg() { return false; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);
    virtual void       collectArgs(TokenList* tokens);



}; // end of NotExpression class

#endif // NOT_EXPRESSION_H
