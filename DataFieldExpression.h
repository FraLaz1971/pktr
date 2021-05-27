#ifndef DATA_FIELD_EXPRESSION_H
#define DATA_FIELD_EXPRESSION_H

#include "Expression.h"

class DataFieldExpression : public Expression {

private:
    /*******
    * data *
    *******/
    string name;
    char type;
    int bytes;
    int bits;
    int size;


public:
    /**************
    * constructor *
    **************/
    DataFieldExpression(char type, int bytes, int bits, int size);

    /*************
    * destructor *
    *************/
    virtual ~DataFieldExpression() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual double evaluate(Context* c) throw(Exception);
    virtual void set(Context* c, double value) throw(Exception);

    virtual void print(ostream& out) const { out << "%"<< name; }


}; // end of DataFieldExpression class

#endif // DATA_FIELD_EXPRESSION_H
