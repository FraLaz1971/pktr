#ifndef ASSIGNMENT_OPERATOR_H
#define ASSIGNMENT_OPERATOR_H

#include "OperatorExpression.h"

class AssignmentOperator : public OperatorExpression {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    AssignmentOperator();

    /*************
    * destructor *
    *************/
    virtual ~AssignmentOperator() {}

    /************
    * accessors *
    ************/
    virtual int precedence() { return 7; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);





}; // end of AssignmentOperator class

#endif // ASSIGNMENT_OPERATOR_H
