// This file was automatically generated from StandardLDPHead.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "StandardLDPHead.h"
#include <sstream>

    unsigned int StandardLDPHead::npages() const { return _npages; }
    SwiftHead3* StandardLDPHead::head3() const { return _head3; }

    void StandardLDPHead::npages(unsigned int value) { _npages = value; }

void StandardLDPHead::read(Reader* r) throw(Interpreter::Exception) {

    _npages = r->readUnsignedInt(2);
    _head3->read(r);

} // end of read method

void StandardLDPHead::write(Writer* w) throw(Interpreter::Exception) {

    w->writeUnsignedInt(_npages,2);
    _head3->write(w);

} // end of write method

const string& StandardLDPHead::toString() {
    ostringstream s;
    s << "npages=" << _npages;
    s << " " << _head3->toString();
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(StandardLDPHead& left, StandardLDPHead& right){

    if(left.npages() != right.npages() ) return false;
    if(*(left.head3()) != *(right.head3()) ) return false;

    return true;
}


int operator!=(StandardLDPHead& left, StandardLDPHead& right){
    return !(left==right);
}
