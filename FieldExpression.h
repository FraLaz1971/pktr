#ifndef FIELD_EXPRESSION_H
#define FIELD_EXPRESSION_H

#include "Expression.h"

#include "SwiftHead3Packet.h"
#include "LDPPacket.h"

/*****************************************************************************
* This class represents a value parsed from the header of a CCSDS packet.
* The currently supported values are described in the documentation for the
* packet_filter tool. Thgis is the only Expression class which actually does
* anything with the CCSDSPacket argument of the evaluate method.
*
*****************************************************************************/
class FieldExpression : public Expression {

private:
    /*******
    * data *
    *******/
    string name;

public:
    /**************
    * constructor *
    **************/
    FieldExpression(const string& name);

    /*************
    * destructor *
    *************/
    virtual ~FieldExpression() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);

    virtual double evaluate(Context* c) throw(Exception);
    virtual void set(Context* c, double value) throw(Exception);

    virtual void print(ostream& out) const { out << "%"<< name; }

protected:
    virtual      SwiftPacket* asHead2(CCSDSPacket* generic_p) throw(Exception);
    virtual SwiftHead3Packet* asHead3(CCSDSPacket* generic_p) throw(Exception);
    virtual        LDPPacket* asLDP(  CCSDSPacket* generic_p) throw(Exception);



}; // end of FieldExpression class

#endif // FIELD_EXPRESSION_H
