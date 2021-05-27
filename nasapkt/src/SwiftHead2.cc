// This file was automatically generated from SwiftHead2.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "SwiftHead2.h"
#include <sstream>

    SwiftTime* SwiftHead2::time() const { return _time; }


void SwiftHead2::read(Reader* r) throw(Interpreter::Exception) {

    _time->read(r);

} // end of read method

void SwiftHead2::write(Writer* w) throw(Interpreter::Exception) {

    _time->write(w);

} // end of write method

const string& SwiftHead2::toString() {
    ostringstream s;
    s << "" << _time->toString();
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(SwiftHead2& left, SwiftHead2& right){

    if(*(left.time()) != *(right.time()) ) return false;

    return true;
}


int operator!=(SwiftHead2& left, SwiftHead2& right){
    return !(left==right);
}
