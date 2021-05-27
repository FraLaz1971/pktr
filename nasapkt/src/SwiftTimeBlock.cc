// This file was automatically generated from SwiftTimeBlock.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "SwiftTimeBlock.h"
#include <sstream>

    unsigned int SwiftTimeBlock::seconds() const { return _seconds; }
    unsigned int SwiftTimeBlock::subseconds() const { return _subseconds; }

    void SwiftTimeBlock::seconds(unsigned int value) { _seconds = value; }
    void SwiftTimeBlock::subseconds(unsigned int value) { _subseconds = value; }

void SwiftTimeBlock::read(Reader* r) throw(Interpreter::Exception) {

    _seconds = r->readUnsignedInt(4);
    _subseconds = r->readUnsignedInt(2);

} // end of read method

void SwiftTimeBlock::write(Writer* w) throw(Interpreter::Exception) {

    w->writeUnsignedInt(_seconds,4);
    w->writeUnsignedInt(_subseconds,2);

} // end of write method

const string& SwiftTimeBlock::toString() {
    ostringstream s;
    s << "seconds=" << _seconds;
    s << " subseconds=" << _subseconds;
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(SwiftTimeBlock& left, SwiftTimeBlock& right){

    if(left.seconds() != right.seconds() ) return false;
    if(left.subseconds() != right.subseconds() ) return false;

    return true;
}


int operator!=(SwiftTimeBlock& left, SwiftTimeBlock& right){
    return !(left==right);
}
