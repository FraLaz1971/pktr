// This file was automatically generated from SwiftHead3.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "SwiftHead3.h"
#include <sstream>

    unsigned int SwiftHead3::obs() const { return _obs; }
    unsigned int SwiftHead3::target() const { return _target; }
    SwiftTime* SwiftHead3::sc_time() const { return _sc_time; }
    SwiftTime* SwiftHead3::utcf_time() const { return _utcf_time; }

    void SwiftHead3::obs(unsigned int value) { _obs = value; }
    void SwiftHead3::target(unsigned int value) { _target = value; }

void SwiftHead3::read(Reader* r) throw(Interpreter::Exception) {

    _obs = r->readUnsignedInt(1);
    _target = r->readUnsignedInt(3);
    _sc_time->read(r);
    _utcf_time->read(r);

} // end of read method

void SwiftHead3::write(Writer* w) throw(Interpreter::Exception) {

    w->writeUnsignedInt(_obs,1);
    w->writeUnsignedInt(_target,3);
    _sc_time->write(w);
    _utcf_time->write(w);

} // end of write method

const string& SwiftHead3::toString() {
    ostringstream s;
    s << "obs=" << _obs;
    s << " target=" << _target;
    s << " " << _sc_time->toString();
    s << " " << _utcf_time->toString();
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(SwiftHead3& left, SwiftHead3& right){

    if(left.obs() != right.obs() ) return false;
    if(left.target() != right.target() ) return false;
    if(*(left.sc_time()) != *(right.sc_time()) ) return false;
    if(*(left.utcf_time()) != *(right.utcf_time()) ) return false;

    return true;
}


int operator!=(SwiftHead3& left, SwiftHead3& right){
    return !(left==right);
}
