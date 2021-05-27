// This file was automatically generated from CCSDSHead.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef CCSDSHEAD_H
#define CCSDSHEAD_H

#include "DataBlock.h"

/*************************************************************
* This class was machine generated from the file CCSDSHead.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for a CCSDS primary header
*************************************************************/
class CCSDSHead : public DataBlock {

public:
    const static int size=6;

private:
    unsigned int _version; // CCSDS format version
    unsigned int _type; // 0=telemetry, 1=telecommand
    unsigned int _hasHead2; // Is there a secondary header?
    unsigned int _apid; // Aplication Process ID
    unsigned int _isLast; // Is the packet the last of a group?
    unsigned int _isFirst; // Is the packet the first of a group?
    unsigned int _sequence; // Packet sequence counter
    unsigned int _length; // Data field size - 1

public:
    CCSDSHead() : DataBlock() {
    } // end of constructor

    CCSDSHead(const CCSDSHead& orig) : DataBlock() {
        _version =  orig.version();
        _type =  orig.type();
        _hasHead2 =  orig.hasHead2();
        _apid =  orig.apid();
        _isLast =  orig.isLast();
        _isFirst =  orig.isFirst();
        _sequence =  orig.sequence();
        _length =  orig.length();
    } // end of copy constructor

    virtual unsigned int version() const;
    virtual unsigned int type() const;
    virtual unsigned int hasHead2() const;
    virtual unsigned int apid() const;
    virtual unsigned int isLast() const;
    virtual unsigned int isFirst() const;
    virtual unsigned int sequence() const;
    virtual unsigned int length() const;

    virtual void version(unsigned int value);
    virtual void type(unsigned int value);
    virtual void hasHead2(unsigned int value);
    virtual void apid(unsigned int value);
    virtual void isLast(unsigned int value);
    virtual void isFirst(unsigned int value);
    virtual void sequence(unsigned int value);
    virtual void length(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(CCSDSHead& left,
                            CCSDSHead& right);

    friend int operator!=(CCSDSHead& left,                             CCSDSHead& right);

}; // end of CCSDSHead class
#endif // CCSDSHEAD_H
