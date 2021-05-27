// This file was automatically generated from BATLDPHead.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "BATLDPHead.h"
#include <sstream>

    StandardLDPHead* BATLDPHead::standard() const { return _standard; }
    SwiftHead3* BATLDPHead::end_head3() const { return _end_head3; }
    unsigned int BATLDPHead::head_size() const { return _head_size; }
    unsigned int BATLDPHead::ldp_type() const { return _ldp_type; }
    unsigned int BATLDPHead::data_type() const { return _data_type; }
    unsigned int BATLDPHead::name0() const { return _name0; }
    unsigned int BATLDPHead::name1() const { return _name1; }
    unsigned int BATLDPHead::name2() const { return _name2; }
    unsigned int BATLDPHead::name3() const { return _name3; }
    unsigned int BATLDPHead::name4() const { return _name4; }
    unsigned int BATLDPHead::name5() const { return _name5; }
    unsigned int BATLDPHead::name6() const { return _name6; }
    unsigned int BATLDPHead::name7() const { return _name7; }
    unsigned int BATLDPHead::name8() const { return _name8; }
    unsigned int BATLDPHead::name9() const { return _name9; }
    unsigned int BATLDPHead::name10() const { return _name10; }
    unsigned int BATLDPHead::name11() const { return _name11; }
    unsigned int BATLDPHead::name12() const { return _name12; }
    unsigned int BATLDPHead::priority() const { return _priority; }
    unsigned int BATLDPHead::ebins() const { return _ebins; }
    ACS* BATLDPHead::start_acs() const { return _start_acs; }
    ACS* BATLDPHead::end_acs() const { return _end_acs; }
    int BATLDPHead::gain() const { return _gain; }
    int BATLDPHead::offset() const { return _offset; }
    unsigned int BATLDPHead::block_ids() const { return _block_ids; }
    unsigned int BATLDPHead::detectors() const { return _detectors; }

    void BATLDPHead::head_size(unsigned int value) { _head_size = value; }
    void BATLDPHead::ldp_type(unsigned int value) { _ldp_type = value; }
    void BATLDPHead::data_type(unsigned int value) { _data_type = value; }
    void BATLDPHead::name0(unsigned int value) { _name0 = value; }
    void BATLDPHead::name1(unsigned int value) { _name1 = value; }
    void BATLDPHead::name2(unsigned int value) { _name2 = value; }
    void BATLDPHead::name3(unsigned int value) { _name3 = value; }
    void BATLDPHead::name4(unsigned int value) { _name4 = value; }
    void BATLDPHead::name5(unsigned int value) { _name5 = value; }
    void BATLDPHead::name6(unsigned int value) { _name6 = value; }
    void BATLDPHead::name7(unsigned int value) { _name7 = value; }
    void BATLDPHead::name8(unsigned int value) { _name8 = value; }
    void BATLDPHead::name9(unsigned int value) { _name9 = value; }
    void BATLDPHead::name10(unsigned int value) { _name10 = value; }
    void BATLDPHead::name11(unsigned int value) { _name11 = value; }
    void BATLDPHead::name12(unsigned int value) { _name12 = value; }
    void BATLDPHead::priority(unsigned int value) { _priority = value; }
    void BATLDPHead::ebins(unsigned int value) { _ebins = value; }
    void BATLDPHead::gain(int value) { _gain = value; }
    void BATLDPHead::offset(int value) { _offset = value; }
    void BATLDPHead::block_ids(unsigned int value) { _block_ids = value; }
    void BATLDPHead::detectors(unsigned int value) { _detectors = value; }

void BATLDPHead::read(Reader* r) throw(Interpreter::Exception) {

    _standard->read(r);
    _end_head3->read(r);
    _head_size = r->readUnsignedInt(2);
    _ldp_type = r->readUnsignedInt(1);
    _data_type = r->readUnsignedInt(1);
    _name0 = r->readUnsignedInt(1);
    _name1 = r->readUnsignedInt(1);
    _name2 = r->readUnsignedInt(1);
    _name3 = r->readUnsignedInt(1);
    _name4 = r->readUnsignedInt(1);
    _name5 = r->readUnsignedInt(1);
    _name6 = r->readUnsignedInt(1);
    _name7 = r->readUnsignedInt(1);
    _name8 = r->readUnsignedInt(1);
    _name9 = r->readUnsignedInt(1);
    _name10 = r->readUnsignedInt(1);
    _name11 = r->readUnsignedInt(1);
    _name12 = r->readUnsignedInt(1);
    _priority = r->readUnsignedInt(1);
    _ebins = r->readUnsignedInt(2);
    _start_acs->read(r);
    _end_acs->read(r);
    _gain = r->readInt(4);
    _offset = r->readInt(4);
    _block_ids = r->readUnsignedInt(2);
    _detectors = r->readUnsignedInt(2);

} // end of read method

void BATLDPHead::write(Writer* w) throw(Interpreter::Exception) {

    _standard->write(w);
    _end_head3->write(w);
    w->writeUnsignedInt(_head_size,2);
    w->writeUnsignedInt(_ldp_type,1);
    w->writeUnsignedInt(_data_type,1);
    w->writeUnsignedInt(_name0,1);
    w->writeUnsignedInt(_name1,1);
    w->writeUnsignedInt(_name2,1);
    w->writeUnsignedInt(_name3,1);
    w->writeUnsignedInt(_name4,1);
    w->writeUnsignedInt(_name5,1);
    w->writeUnsignedInt(_name6,1);
    w->writeUnsignedInt(_name7,1);
    w->writeUnsignedInt(_name8,1);
    w->writeUnsignedInt(_name9,1);
    w->writeUnsignedInt(_name10,1);
    w->writeUnsignedInt(_name11,1);
    w->writeUnsignedInt(_name12,1);
    w->writeUnsignedInt(_priority,1);
    w->writeUnsignedInt(_ebins,2);
    _start_acs->write(w);
    _end_acs->write(w);
    w->writeInt(_gain,4);
    w->writeInt(_offset,4);
    w->writeUnsignedInt(_block_ids,2);
    w->writeUnsignedInt(_detectors,2);

} // end of write method

const string& BATLDPHead::toString() {
    ostringstream s;
    s << "" << _standard->toString();
    s << " " << _end_head3->toString();
    s << " head_size=" << _head_size;
    s << " ldp_type=" << _ldp_type;
    s << " data_type=" << _data_type;
    s << " name0=" << _name0;
    s << " name1=" << _name1;
    s << " name2=" << _name2;
    s << " name3=" << _name3;
    s << " name4=" << _name4;
    s << " name5=" << _name5;
    s << " name6=" << _name6;
    s << " name7=" << _name7;
    s << " name8=" << _name8;
    s << " name9=" << _name9;
    s << " name10=" << _name10;
    s << " name11=" << _name11;
    s << " name12=" << _name12;
    s << " priority=" << _priority;
    s << " ebins=" << _ebins;
    s << " " << _start_acs->toString();
    s << " " << _end_acs->toString();
    s << " gain=" << _gain;
    s << " offset=" << _offset;
    s << " block_ids=" << _block_ids;
    s << " detectors=" << _detectors;
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(BATLDPHead& left, BATLDPHead& right){

    if(*(left.standard()) != *(right.standard()) ) return false;
    if(*(left.end_head3()) != *(right.end_head3()) ) return false;
    if(left.head_size() != right.head_size() ) return false;
    if(left.ldp_type() != right.ldp_type() ) return false;
    if(left.data_type() != right.data_type() ) return false;
    if(left.name0() != right.name0() ) return false;
    if(left.name1() != right.name1() ) return false;
    if(left.name2() != right.name2() ) return false;
    if(left.name3() != right.name3() ) return false;
    if(left.name4() != right.name4() ) return false;
    if(left.name5() != right.name5() ) return false;
    if(left.name6() != right.name6() ) return false;
    if(left.name7() != right.name7() ) return false;
    if(left.name8() != right.name8() ) return false;
    if(left.name9() != right.name9() ) return false;
    if(left.name10() != right.name10() ) return false;
    if(left.name11() != right.name11() ) return false;
    if(left.name12() != right.name12() ) return false;
    if(left.priority() != right.priority() ) return false;
    if(left.ebins() != right.ebins() ) return false;
    if(*(left.start_acs()) != *(right.start_acs()) ) return false;
    if(*(left.end_acs()) != *(right.end_acs()) ) return false;
    if(left.gain() != right.gain() ) return false;
    if(left.offset() != right.offset() ) return false;
    if(left.block_ids() != right.block_ids() ) return false;
    if(left.detectors() != right.detectors() ) return false;

    return true;
}


int operator!=(BATLDPHead& left, BATLDPHead& right){
    return !(left==right);
}
