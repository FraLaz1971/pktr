// This file was automatically generated from LDPTrailer.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "LDPTrailer.h"
#include <sstream>

    unsigned int LDPTrailer::checksum() const { return _checksum; }

    void LDPTrailer::checksum(unsigned int value) { _checksum = value; }

void LDPTrailer::read(Reader* r) throw(Interpreter::Exception) {

    _checksum = r->readUnsignedInt(2);

} // end of read method

void LDPTrailer::write(Writer* w) throw(Interpreter::Exception) {

    w->writeUnsignedInt(_checksum,2);

} // end of write method

const string& LDPTrailer::toString() {
    ostringstream s;
    s << "checksum=" << _checksum;
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(LDPTrailer& left, LDPTrailer& right){

    if(left.checksum() != right.checksum() ) return false;

    return true;
}


int operator!=(LDPTrailer& left, LDPTrailer& right){
    return !(left==right);
}
