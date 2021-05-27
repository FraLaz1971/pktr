// This file was automatically generated from StandardLDPHead.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef STANDARD_LDPHEAD_H
#define STANDARD_LDPHEAD_H

#include "DataBlock.h"
#include "SwiftHead3.h"

/*************************************************************
* This class was machine generated from the file StandardLDPHead.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for 
* the product and page number fields of a Swift LDP packet
*************************************************************/
class StandardLDPHead : public DataBlock {

public:
    const static int size=18;

private:
    unsigned int _npages; // Number of pages in this product
    SwiftHead3* _head3; // Swift tertiary header

public:
    StandardLDPHead() : DataBlock() {
        _head3 = new SwiftHead3();
    } // end of constructor

    StandardLDPHead(const StandardLDPHead& orig) : DataBlock() {
        _npages =  orig.npages();
        *_head3 =  *(orig.head3());
    } // end of copy constructor

    virtual unsigned int npages() const;
    virtual SwiftHead3* head3() const;

    virtual void npages(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(StandardLDPHead& left,
                            StandardLDPHead& right);

    friend int operator!=(StandardLDPHead& left,                             StandardLDPHead& right);

}; // end of StandardLDPHead class
#endif // STANDARD_LDPHEAD_H
