#ifndef CONSTANT_EXPRESSION_H
#define CONSTANT_EXPRESSION_H

#include "Expression.h"

/*****************************************************************************
* This class represents a single double precision floating point number
*****************************************************************************/
class ConstantExpression : public Expression {

private:
    /*******
    * data *
    *******/
    double value;


public:
    /**************
    * constructor *
    **************/
    ConstantExpression(double value);

    /*************
    * destructor *
    *************/
    virtual ~ConstantExpression() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception) { return value; }

    virtual void print(ostream& out) const { out << value; }




}; // end of ConstantExpression class

#endif // CONSTANT_EXPRESSION_H
