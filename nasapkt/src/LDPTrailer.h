// This file was automatically generated from LDPTrailer.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef LDPTRAILER_H
#define LDPTRAILER_H

#include "DataBlock.h"

/*************************************************************
* This class was machine generated from the file LDPTrailer.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for a
* Swift Large Data Product checksum packet trailer
*************************************************************/
class LDPTrailer : public DataBlock {

public:
    const static int size=2;

private:
    unsigned int _checksum; // Packet checksum

public:
    LDPTrailer() : DataBlock() {
    } // end of constructor

    LDPTrailer(const LDPTrailer& orig) : DataBlock() {
        _checksum =  orig.checksum();
    } // end of copy constructor

    virtual unsigned int checksum() const;

    virtual void checksum(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(LDPTrailer& left,
                            LDPTrailer& right);

    friend int operator!=(LDPTrailer& left,                             LDPTrailer& right);

}; // end of LDPTrailer class
#endif // LDPTRAILER_H
