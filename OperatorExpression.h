#ifndef OPERATOR_EXPRESSION_H
#define OPERATOR_EXPRESSION_H

#include "Expression.h"

/*****************************************************************************
* This is an abstract superclass for all Expressions which have arguments.
* There is basic support for binary operators with arguments to the left
* and right, as well as unary operators with an argument to the right only.
* The methods default to the correct behavior for binary operators.
******************************************************************************/
class OperatorExpression : public Expression {

protected:
    /*******
    * data *
    *******/
    string name;


public:
    /**************
    * constructor *
    **************/
    OperatorExpression();

    /*************
    * destructor *
    *************/
    virtual ~OperatorExpression() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual bool hasArgs() { return true; }
    virtual bool hasLeftArg() { return true; } // binary by default
    virtual bool hasRightArg() { return true; }

    virtual void print(ostream& out) const;

    virtual void       collectArgs(TokenList* tokens);
    virtual void collectBinaryArgs(TokenList* tokens);
    virtual void  collectUnaryArgs(TokenList* tokens);


}; // end of OperatorExpression class

#endif // OPERATOR_EXPRESSION_H
