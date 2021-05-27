// This file was automatically generated from LDPHead3.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef LDPHEAD3_H
#define LDPHEAD3_H

#include "DataBlock.h"

/*************************************************************
* This class was machine generated from the file LDPHead3.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for 
* the product and page number fields of a Swift LDP packet
*************************************************************/
class LDPHead3 : public DataBlock {

public:
    const static int size=4;

private:
    unsigned int _product; // ID for this large data product
    unsigned int _page; // Packet counter within LDP

public:
    LDPHead3() : DataBlock() {
    } // end of constructor

    LDPHead3(const LDPHead3& orig) : DataBlock() {
        _product =  orig.product();
        _page =  orig.page();
    } // end of copy constructor

    virtual unsigned int product() const;
    virtual unsigned int page() const;

    virtual void product(unsigned int value);
    virtual void page(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(LDPHead3& left,
                            LDPHead3& right);

    friend int operator!=(LDPHead3& left,                             LDPHead3& right);

}; // end of LDPHead3 class
#endif // LDPHEAD3_H
