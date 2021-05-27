// This file was automatically generated from SwiftHead3.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef SWIFT_HEAD3_H
#define SWIFT_HEAD3_H

#include "DataBlock.h"
#include "SwiftTime.h"
#include "SwiftTime.h"

/*************************************************************
* This class was machine generated from the file SwiftHead3.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for a Swift tertiary header
*************************************************************/
class SwiftHead3 : public DataBlock {

public:
    const static int size=16;

private:
    unsigned int _obs; // Observation Segment ID
    unsigned int _target; // Target ID
    SwiftTime* _sc_time; // Spacecraft Science time
    SwiftTime* _utcf_time; // Corrected Science time

public:
    SwiftHead3() : DataBlock() {
        _sc_time = new SwiftTime();
        _utcf_time = new SwiftTime();
    } // end of constructor

    SwiftHead3(const SwiftHead3& orig) : DataBlock() {
        _obs =  orig.obs();
        _target =  orig.target();
        *_sc_time =  *(orig.sc_time());
        *_utcf_time =  *(orig.utcf_time());
    } // end of copy constructor

    virtual unsigned int obs() const;
    virtual unsigned int target() const;
    virtual SwiftTime* sc_time() const;
    virtual SwiftTime* utcf_time() const;

    virtual void obs(unsigned int value);
    virtual void target(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(SwiftHead3& left,
                            SwiftHead3& right);

    friend int operator!=(SwiftHead3& left,                             SwiftHead3& right);

}; // end of SwiftHead3 class
#endif // SWIFT_HEAD3_H
