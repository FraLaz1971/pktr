// This file was automatically generated from SwiftTimeBlock.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef SWIFT_TIME_BLOCK_H
#define SWIFT_TIME_BLOCK_H

#include "DataBlock.h"

/*************************************************************
* This class was machine generated from the file SwiftTimeBlock.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for a Swift time record
*************************************************************/
class SwiftTimeBlock : public DataBlock {

public:
    const static int size=6;

private:
    unsigned int _seconds; // Seconds field
    unsigned int _subseconds; // Fractional seconds field

public:
    SwiftTimeBlock() : DataBlock() {
    } // end of constructor

    SwiftTimeBlock(const SwiftTimeBlock& orig) : DataBlock() {
        _seconds =  orig.seconds();
        _subseconds =  orig.subseconds();
    } // end of copy constructor

    virtual unsigned int seconds() const;
    virtual unsigned int subseconds() const;

    virtual void seconds(unsigned int value);
    virtual void subseconds(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(SwiftTimeBlock& left,
                            SwiftTimeBlock& right);

    friend int operator!=(SwiftTimeBlock& left,                             SwiftTimeBlock& right);

}; // end of SwiftTimeBlock class
#endif // SWIFT_TIME_BLOCK_H
