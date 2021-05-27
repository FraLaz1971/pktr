// This file was automatically generated from ACS.db.
// Do not Edit! Instead, create a subclass to extend functionality

#include "ACS.h"
#include <sstream>

    SwiftTime* ACS::time() const { return _time; }
    unsigned int ACS::fill() const { return _fill; }
    unsigned int ACS::obs() const { return _obs; }
    unsigned int ACS::target() const { return _target; }
    double ACS::ra() const { return _ra; }
    double ACS::dec() const { return _dec; }
    float ACS::roll() const { return _roll; }
    float ACS::lat() const { return _lat; }
    float ACS::lon() const { return _lon; }
    float ACS::alt() const { return _alt; }
    float ACS::bus() const { return _bus; }
    unsigned char ACS::saa() const { return _saa; }
    unsigned char ACS::ten_arcmin() const { return _ten_arcmin; }
    unsigned char ACS::settled() const { return _settled; }
    unsigned char ACS::safemode() const { return _safemode; }

    void ACS::fill(unsigned int value) { _fill = value; }
    void ACS::obs(unsigned int value) { _obs = value; }
    void ACS::target(unsigned int value) { _target = value; }
    void ACS::ra(double value) { _ra = value; }
    void ACS::dec(double value) { _dec = value; }
    void ACS::roll(float value) { _roll = value; }
    void ACS::lat(float value) { _lat = value; }
    void ACS::lon(float value) { _lon = value; }
    void ACS::alt(float value) { _alt = value; }
    void ACS::bus(float value) { _bus = value; }
    void ACS::saa(unsigned char value) { _saa = value; }
    void ACS::ten_arcmin(unsigned char value) { _ten_arcmin = value; }
    void ACS::settled(unsigned char value) { _settled = value; }
    void ACS::safemode(unsigned char value) { _safemode = value; }

void ACS::read(Reader* r) throw(Interpreter::Exception) {

    _time->read(r);
    _fill = r->readUnsignedInt(2);
    _obs = r->readUnsignedInt(1);
    _target = r->readUnsignedInt(3);
    _ra = r->readDouble();
    _dec = r->readDouble();
    _roll = r->readFloat();
    _lat = r->readFloat();
    _lon = r->readFloat();
    _alt = r->readFloat();
    _bus = r->readFloat();
    _saa = r->readByte();
    _ten_arcmin = r->readByte();
    _settled = r->readByte();
    _safemode = r->readByte();

} // end of read method

void ACS::write(Writer* w) throw(Interpreter::Exception) {

    _time->write(w);
    w->writeUnsignedInt(_fill,2);
    w->writeUnsignedInt(_obs,1);
    w->writeUnsignedInt(_target,3);
    w->writeDouble(_ra);
    w->writeDouble(_dec);
    w->writeFloat(_roll);
    w->writeFloat(_lat);
    w->writeFloat(_lon);
    w->writeFloat(_alt);
    w->writeFloat(_bus);
    w->writeByte(_saa);
    w->writeByte(_ten_arcmin);
    w->writeByte(_settled);
    w->writeByte(_safemode);

} // end of write method

const string& ACS::toString() {
    ostringstream s;
    s << "" << _time->toString();
    s << " fill=" << _fill;
    s << " obs=" << _obs;
    s << " target=" << _target;
    s << " ra=" << _ra;
    s << " dec=" << _dec;
    s << " roll=" << _roll;
    s << " lat=" << _lat;
    s << " lon=" << _lon;
    s << " alt=" << _alt;
    s << " bus=" << _bus;
    s << " saa=" << _saa;
    s << " ten_arcmin=" << _ten_arcmin;
    s << " settled=" << _settled;
    s << " safemode=" << _safemode;
#   ifdef FAKING_STRINGSTREAM
    s << ends;
#   endif
    line = s.str();
    return line;
} // end of toString method


int operator==(ACS& left, ACS& right){

    if(*(left.time()) != *(right.time()) ) return false;
    if(left.fill() != right.fill() ) return false;
    if(left.obs() != right.obs() ) return false;
    if(left.target() != right.target() ) return false;
    if(left.ra() != right.ra() ) return false;
    if(left.dec() != right.dec() ) return false;
    if(left.roll() != right.roll() ) return false;
    if(left.lat() != right.lat() ) return false;
    if(left.lon() != right.lon() ) return false;
    if(left.alt() != right.alt() ) return false;
    if(left.bus() != right.bus() ) return false;
    if(left.saa() != right.saa() ) return false;
    if(left.ten_arcmin() != right.ten_arcmin() ) return false;
    if(left.settled() != right.settled() ) return false;
    if(left.safemode() != right.safemode() ) return false;

    return true;
}


int operator!=(ACS& left, ACS& right){
    return !(left==right);
}
