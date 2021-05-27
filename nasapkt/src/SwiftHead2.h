// This file was automatically generated from SwiftHead2.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef SWIFT_HEAD2_H
#define SWIFT_HEAD2_H

#include "DataBlock.h"
#include "SwiftTime.h"

/*************************************************************
* This class was machine generated from the file SwiftHead2.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for a Swift secondary header
*************************************************************/
class SwiftHead2 : public DataBlock {

public:
    const static int size=6;

private:
    SwiftTime* _time; // Packet creation time

public:
    SwiftHead2() : DataBlock() {
        _time = new SwiftTime();
    } // end of constructor

    SwiftHead2(const SwiftHead2& orig) : DataBlock() {
        *_time =  *(orig.time());
    } // end of copy constructor

    virtual SwiftTime* time() const;


    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(SwiftHead2& left,
                            SwiftHead2& right);

    friend int operator!=(SwiftHead2& left,                             SwiftHead2& right);

}; // end of SwiftHead2 class
#endif // SWIFT_HEAD2_H
