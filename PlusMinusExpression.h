#ifndef PLUS_MINUS_EXPRESSION_H
#define PLUS_MINUS_EXPRESSION_H

#include "OperatorExpression.h"

class PlusMinusExpression : public OperatorExpression {

private:
    /*******
    * data *
    *******/
    bool positive;
    bool unary;


public:
    /**************
    * constructor *
    **************/
    PlusMinusExpression(bool positive);

    /*************
    * destructor *
    *************/
    virtual ~PlusMinusExpression() {}

    /************
    * accessors *
    ************/
    virtual int precedence();
    virtual bool hasLeftArg() { return !unary; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);

    virtual void firstPass(TokenList* tokens);
    virtual void collectArgs(TokenList* tokens);




}; // end of PlusMinusExpression class

#endif // PLUS_MINUS_EXPRESSION_H
