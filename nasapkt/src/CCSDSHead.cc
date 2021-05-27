// This file was automatically generated from CCSDSHead.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "CCSDSHead.h"
#include <sstream>

    unsigned int CCSDSHead::version() const { return _version; }
    unsigned int CCSDSHead::type() const { return _type; }
    unsigned int CCSDSHead::hasHead2() const { return _hasHead2; }
    unsigned int CCSDSHead::apid() const { return _apid; }
    unsigned int CCSDSHead::isLast() const { return _isLast; }
    unsigned int CCSDSHead::isFirst() const { return _isFirst; }
    unsigned int CCSDSHead::sequence() const { return _sequence; }
    unsigned int CCSDSHead::length() const { return _length; }

    void CCSDSHead::version(unsigned int value) { _version = value; }
    void CCSDSHead::type(unsigned int value) { _type = value; }
    void CCSDSHead::hasHead2(unsigned int value) { _hasHead2 = value; }
    void CCSDSHead::apid(unsigned int value) { _apid = value; }
    void CCSDSHead::isLast(unsigned int value) { _isLast = value; }
    void CCSDSHead::isFirst(unsigned int value) { _isFirst = value; }
    void CCSDSHead::sequence(unsigned int value) { _sequence = value; }
    void CCSDSHead::length(unsigned int value) { _length = value; }

void CCSDSHead::read(Reader* r) throw(Interpreter::Exception) {

    _version = r->readUnsignedBits(3);
    _type = r->readUnsignedBits(1);
    _hasHead2 = r->readUnsignedBits(1);
    _apid = r->readUnsignedBits(11);
    _isLast = r->readUnsignedBits(1);
    _isFirst = r->readUnsignedBits(1);
    _sequence = r->readUnsignedBits(14);
    _length = r->readUnsignedInt(2);

} // end of read method

void CCSDSHead::write(Writer* w) throw(Interpreter::Exception) {

    w->writeUnsignedBits(_version,3);
    w->writeUnsignedBits(_type,1);
    w->writeUnsignedBits(_hasHead2,1);
    w->writeUnsignedBits(_apid,11);
    w->writeUnsignedBits(_isLast,1);
    w->writeUnsignedBits(_isFirst,1);
    w->writeUnsignedBits(_sequence,14);
    w->writeUnsignedInt(_length,2);

} // end of write method

const string& CCSDSHead::toString() {
    ostringstream s;
    s << "version=" << _version;
    s << " type=" << _type;
    s << " hasHead2=" << _hasHead2;
    s << " apid=" << _apid;
    s << " isLast=" << _isLast;
    s << " isFirst=" << _isFirst;
    s << " sequence=" << _sequence;
    s << " length=" << _length;
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(CCSDSHead& left, CCSDSHead& right){

    if(left.version() != right.version() ) return false;
    if(left.type() != right.type() ) return false;
    if(left.hasHead2() != right.hasHead2() ) return false;
    if(left.apid() != right.apid() ) return false;
    if(left.isLast() != right.isLast() ) return false;
    if(left.isFirst() != right.isFirst() ) return false;
    if(left.sequence() != right.sequence() ) return false;
    if(left.length() != right.length() ) return false;

    return true;
}


int operator!=(CCSDSHead& left, CCSDSHead& right){
    return !(left==right);
}
