// This file was automatically generated from ACS.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef ACS_H
#define ACS_H

#include "DataBlock.h"
#include "SwiftTime.h"

/*************************************************************
* This class was machine generated from the file ACS.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* Packet type: HEAD2
* BAT echos of ACS packets.
* This description does not include the checksum which appears at the end 
* of the ACS packets.
*************************************************************/
class ACS : public DataBlock {

public:
    const static int size=52;

private:
    SwiftTime* _time; // Attitude time stamp
    unsigned int _fill; // Unused
    unsigned int _obs; // Observation Segment ID
    unsigned int _target; // Target ID
    double _ra; // Right Ascension (deg)
    double _dec; // Declination (deg)
    float _roll; // Roll Angle (deg)
    float _lat; // Latitude (deg)
    float _lon; // Longitude (deg)
    float _alt; // Altitude (m)
    float _bus; // Spacecraft bus voltage
    unsigned char _saa; // 1 => in SAA
    unsigned char _ten_arcmin; // 1 => within 10 arcmin of target
    unsigned char _settled; // 1 => is settled on target
    unsigned char _safemode; // 1 => in Safe Mode

public:
    ACS() : DataBlock() {
        _time = new SwiftTime();
    } // end of constructor

    ACS(const ACS& orig) : DataBlock() {
        *_time =  *(orig.time());
        _fill =  orig.fill();
        _obs =  orig.obs();
        _target =  orig.target();
        _ra =  orig.ra();
        _dec =  orig.dec();
        _roll =  orig.roll();
        _lat =  orig.lat();
        _lon =  orig.lon();
        _alt =  orig.alt();
        _bus =  orig.bus();
        _saa =  orig.saa();
        _ten_arcmin =  orig.ten_arcmin();
        _settled =  orig.settled();
        _safemode =  orig.safemode();
    } // end of copy constructor

    virtual SwiftTime* time() const;
    virtual unsigned int fill() const;
    virtual unsigned int obs() const;
    virtual unsigned int target() const;
    virtual double ra() const;
    virtual double dec() const;
    virtual float roll() const;
    virtual float lat() const;
    virtual float lon() const;
    virtual float alt() const;
    virtual float bus() const;
    virtual unsigned char saa() const;
    virtual unsigned char ten_arcmin() const;
    virtual unsigned char settled() const;
    virtual unsigned char safemode() const;

    virtual void fill(unsigned int value);
    virtual void obs(unsigned int value);
    virtual void target(unsigned int value);
    virtual void ra(double value);
    virtual void dec(double value);
    virtual void roll(float value);
    virtual void lat(float value);
    virtual void lon(float value);
    virtual void alt(float value);
    virtual void bus(float value);
    virtual void saa(unsigned char value);
    virtual void ten_arcmin(unsigned char value);
    virtual void settled(unsigned char value);
    virtual void safemode(unsigned char value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(ACS& left,
                            ACS& right);

    friend int operator!=(ACS& left,                             ACS& right);

}; // end of ACS class
#endif // ACS_H
