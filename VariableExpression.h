#ifndef VARIABLE_EXPRESSION_H
#define VARIABLE_EXPRESSION_H

#include "Expression.h"

/*****************************************************************************
* This class represents a value parsed from the header of a CCSDS packet.
* The currently supported values are described in the documentation for the
* packet_filter tool. Thgis is the only Expression class which actually does
* anything with the CCSDSPacket argument of the evaluate method.
*
*****************************************************************************/
class VariableExpression : public Expression {

private:
    /*******
    * data *
    *******/
    string name;

public:
    /**************
    * constructor *
    **************/
    VariableExpression(const string& name);

    /*************
    * destructor *
    *************/
    virtual ~VariableExpression() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);

    virtual double evaluate(Context* c) throw(Exception);
    virtual void set(Context* c, double value) throw(Exception);

    virtual void print(ostream& out) const { out << "$"<<name; }



}; // end of FieldExpression class

#endif // VARIABLE_EXPRESSION_H
