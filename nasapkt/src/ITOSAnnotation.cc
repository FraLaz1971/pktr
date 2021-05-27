// This file was automatically generated from ITOSAnnotation.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "ITOSAnnotation.h"
#include <sstream>

    unsigned int ITOSAnnotation::version() const { return _version; }
    unsigned int ITOSAnnotation::spacecraft() const { return _spacecraft; }
    unsigned int ITOSAnnotation::vc() const { return _vc; }
    unsigned int ITOSAnnotation::reserved() const { return _reserved; }
    unsigned int ITOSAnnotation::rs_enabled() const { return _rs_enabled; }
    unsigned int ITOSAnnotation::rs_error() const { return _rs_error; }
    unsigned int ITOSAnnotation::rs_corrected() const { return _rs_corrected; }
    unsigned int ITOSAnnotation::time_format() const { return _time_format; }
    unsigned int ITOSAnnotation::head_error() const { return _head_error; }
    unsigned int ITOSAnnotation::reversed() const { return _reversed; }
    unsigned int ITOSAnnotation::seq_error() const { return _seq_error; }
    unsigned int ITOSAnnotation::frame_error() const { return _frame_error; }
    unsigned int ITOSAnnotation::checking() const { return _checking; }
    unsigned int ITOSAnnotation::incomplete() const { return _incomplete; }
    unsigned int ITOSAnnotation::vc_seq_error() const { return _vc_seq_error; }
    unsigned int ITOSAnnotation::frame_head_error() const { return _frame_head_error; }
    unsigned int ITOSAnnotation::fill() const { return _fill; }
    unsigned int ITOSAnnotation::time() const { return _time; }
    unsigned int ITOSAnnotation::time2() const { return _time2; }

    void ITOSAnnotation::version(unsigned int value) { _version = value; }
    void ITOSAnnotation::spacecraft(unsigned int value) { _spacecraft = value; }
    void ITOSAnnotation::vc(unsigned int value) { _vc = value; }
    void ITOSAnnotation::reserved(unsigned int value) { _reserved = value; }
    void ITOSAnnotation::rs_enabled(unsigned int value) { _rs_enabled = value; }
    void ITOSAnnotation::rs_error(unsigned int value) { _rs_error = value; }
    void ITOSAnnotation::rs_corrected(unsigned int value) { _rs_corrected = value; }
    void ITOSAnnotation::time_format(unsigned int value) { _time_format = value; }
    void ITOSAnnotation::head_error(unsigned int value) { _head_error = value; }
    void ITOSAnnotation::reversed(unsigned int value) { _reversed = value; }
    void ITOSAnnotation::seq_error(unsigned int value) { _seq_error = value; }
    void ITOSAnnotation::frame_error(unsigned int value) { _frame_error = value; }
    void ITOSAnnotation::checking(unsigned int value) { _checking = value; }
    void ITOSAnnotation::incomplete(unsigned int value) { _incomplete = value; }
    void ITOSAnnotation::vc_seq_error(unsigned int value) { _vc_seq_error = value; }
    void ITOSAnnotation::frame_head_error(unsigned int value) { _frame_head_error = value; }
    void ITOSAnnotation::fill(unsigned int value) { _fill = value; }
    void ITOSAnnotation::time(unsigned int value) { _time = value; }
    void ITOSAnnotation::time2(unsigned int value) { _time2 = value; }

void ITOSAnnotation::read(Reader* r) throw(Interpreter::Exception) {

    _version = r->readUnsignedBits(2);
    _spacecraft = r->readUnsignedBits(10);
    _vc = r->readUnsignedBits(3);
    _reserved = r->readUnsignedBits(1);
    _rs_enabled = r->readUnsignedBits(1);
    _rs_error = r->readUnsignedBits(1);
    _rs_corrected = r->readUnsignedBits(1);
    _time_format = r->readUnsignedBits(4);
    r->readUnsignedBits(1); // skipping 1 bit gap
    _head_error = r->readUnsignedBits(1);
    _reversed = r->readUnsignedBits(1);
    _seq_error = r->readUnsignedBits(1);
    _frame_error = r->readUnsignedBits(1);
    _checking = r->readUnsignedBits(1);
    _incomplete = r->readUnsignedBits(1);
    _vc_seq_error = r->readUnsignedBits(1);
    _frame_head_error = r->readUnsignedBits(1);
    _fill = r->readUnsignedInt(2);
    _time = r->readUnsignedInt(4);
    _time2 = r->readUnsignedInt(2);

} // end of read method

void ITOSAnnotation::write(Writer* w) throw(Interpreter::Exception) {

    w->writeUnsignedBits(_version,2);
    w->writeUnsignedBits(_spacecraft,10);
    w->writeUnsignedBits(_vc,3);
    w->writeUnsignedBits(_reserved,1);
    w->writeUnsignedBits(_rs_enabled,1);
    w->writeUnsignedBits(_rs_error,1);
    w->writeUnsignedBits(_rs_corrected,1);
    w->writeUnsignedBits(_time_format,4);
    w->writeUnsignedBits(0,1); // filling 1 bit gap
    w->writeUnsignedBits(_head_error,1);
    w->writeUnsignedBits(_reversed,1);
    w->writeUnsignedBits(_seq_error,1);
    w->writeUnsignedBits(_frame_error,1);
    w->writeUnsignedBits(_checking,1);
    w->writeUnsignedBits(_incomplete,1);
    w->writeUnsignedBits(_vc_seq_error,1);
    w->writeUnsignedBits(_frame_head_error,1);
    w->writeUnsignedInt(_fill,2);
    w->writeUnsignedInt(_time,4);
    w->writeUnsignedInt(_time2,2);

} // end of write method

const string& ITOSAnnotation::toString() {
    ostringstream s;
    s << "version=" << _version;
    s << " spacecraft=" << _spacecraft;
    s << " vc=" << _vc;
    s << " reserved=" << _reserved;
    s << " rs_enabled=" << _rs_enabled;
    s << " rs_error=" << _rs_error;
    s << " rs_corrected=" << _rs_corrected;
    s << " time_format=" << _time_format;
    s << " head_error=" << _head_error;
    s << " reversed=" << _reversed;
    s << " seq_error=" << _seq_error;
    s << " frame_error=" << _frame_error;
    s << " checking=" << _checking;
    s << " incomplete=" << _incomplete;
    s << " vc_seq_error=" << _vc_seq_error;
    s << " frame_head_error=" << _frame_head_error;
    s << " fill=" << _fill;
    s << " time=" << _time;
    s << " time2=" << _time2;
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(ITOSAnnotation& left, ITOSAnnotation& right){

    if(left.version() != right.version() ) return false;
    if(left.spacecraft() != right.spacecraft() ) return false;
    if(left.vc() != right.vc() ) return false;
    if(left.reserved() != right.reserved() ) return false;
    if(left.rs_enabled() != right.rs_enabled() ) return false;
    if(left.rs_error() != right.rs_error() ) return false;
    if(left.rs_corrected() != right.rs_corrected() ) return false;
    if(left.time_format() != right.time_format() ) return false;
    if(left.head_error() != right.head_error() ) return false;
    if(left.reversed() != right.reversed() ) return false;
    if(left.seq_error() != right.seq_error() ) return false;
    if(left.frame_error() != right.frame_error() ) return false;
    if(left.checking() != right.checking() ) return false;
    if(left.incomplete() != right.incomplete() ) return false;
    if(left.vc_seq_error() != right.vc_seq_error() ) return false;
    if(left.frame_head_error() != right.frame_head_error() ) return false;
    if(left.fill() != right.fill() ) return false;
    if(left.time() != right.time() ) return false;
    if(left.time2() != right.time2() ) return false;

    return true;
}


int operator!=(ITOSAnnotation& left, ITOSAnnotation& right){
    return !(left==right);
}
