// This file was automatically generated from LDPHead3.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "LDPHead3.h"
#include <sstream>

    unsigned int LDPHead3::product() const { return _product; }
    unsigned int LDPHead3::page() const { return _page; }

    void LDPHead3::product(unsigned int value) { _product = value; }
    void LDPHead3::page(unsigned int value) { _page = value; }

void LDPHead3::read(Reader* r) throw(Interpreter::Exception) {

    _product = r->readUnsignedInt(2);
    _page = r->readUnsignedInt(2);

} // end of read method

void LDPHead3::write(Writer* w) throw(Interpreter::Exception) {

    w->writeUnsignedInt(_product,2);
    w->writeUnsignedInt(_page,2);

} // end of write method

const string& LDPHead3::toString() {
    ostringstream s;
    s << "product=" << _product;
    s << " page=" << _page;
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(LDPHead3& left, LDPHead3& right){

    if(left.product() != right.product() ) return false;
    if(left.page() != right.page() ) return false;

    return true;
}


int operator!=(LDPHead3& left, LDPHead3& right){
    return !(left==right);
}
