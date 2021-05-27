// This file was automatically generated from BATLDPHead.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef BATLDPHEAD_H
#define BATLDPHEAD_H

#include "DataBlock.h"
#include "StandardLDPHead.h"
#include "SwiftHead3.h"
#include "ACS.h"
#include "ACS.h"

/*************************************************************
* This class was machine generated from the file BATLDPHead.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is the standard header of a BAT science LDP
*************************************************************/
class BATLDPHead : public DataBlock {

public:
    const static int size=174;

private:
    StandardLDPHead* _standard; // standard part of header
    SwiftHead3* _end_head3; // Tertiary header at end
    unsigned int _head_size; // Size of LDp header in bytes
    unsigned int _ldp_type; // LDP type
    unsigned int _data_type; // LDP contents data type
    unsigned int _name0; // 8.3 filename
    unsigned int _name1; // 8.3 filename
    unsigned int _name2; // 8.3 filename
    unsigned int _name3; // 8.3 filename
    unsigned int _name4; // 8.3 filename
    unsigned int _name5; // 8.3 filename
    unsigned int _name6; // 8.3 filename
    unsigned int _name7; // 8.3 filename
    unsigned int _name8; // 8.3 filename
    unsigned int _name9; // 8.3 filename
    unsigned int _name10; // 8.3 filename
    unsigned int _name11; // 8.3 filename
    unsigned int _name12; // 8.3 filename
    unsigned int _priority; // File priority
    unsigned int _ebins; // Number of energy bins
    ACS* _start_acs; // ACS record at start
    ACS* _end_acs; // ACS record at end
    int _gain; // Gain index
    int _offset; // Offset index
    unsigned int _block_ids; // Block ID map
    unsigned int _detectors; // Number of Detectors

public:
    BATLDPHead() : DataBlock() {
        _standard = new StandardLDPHead();
        _end_head3 = new SwiftHead3();
        _start_acs = new ACS();
        _end_acs = new ACS();
    } // end of constructor

    BATLDPHead(const BATLDPHead& orig) : DataBlock() {
        *_standard =  *(orig.standard());
        *_end_head3 =  *(orig.end_head3());
        _head_size =  orig.head_size();
        _ldp_type =  orig.ldp_type();
        _data_type =  orig.data_type();
        _name0 =  orig.name0();
        _name1 =  orig.name1();
        _name2 =  orig.name2();
        _name3 =  orig.name3();
        _name4 =  orig.name4();
        _name5 =  orig.name5();
        _name6 =  orig.name6();
        _name7 =  orig.name7();
        _name8 =  orig.name8();
        _name9 =  orig.name9();
        _name10 =  orig.name10();
        _name11 =  orig.name11();
        _name12 =  orig.name12();
        _priority =  orig.priority();
        _ebins =  orig.ebins();
        *_start_acs =  *(orig.start_acs());
        *_end_acs =  *(orig.end_acs());
        _gain =  orig.gain();
        _offset =  orig.offset();
        _block_ids =  orig.block_ids();
        _detectors =  orig.detectors();
    } // end of copy constructor

    virtual StandardLDPHead* standard() const;
    virtual SwiftHead3* end_head3() const;
    virtual unsigned int head_size() const;
    virtual unsigned int ldp_type() const;
    virtual unsigned int data_type() const;
    virtual unsigned int name0() const;
    virtual unsigned int name1() const;
    virtual unsigned int name2() const;
    virtual unsigned int name3() const;
    virtual unsigned int name4() const;
    virtual unsigned int name5() const;
    virtual unsigned int name6() const;
    virtual unsigned int name7() const;
    virtual unsigned int name8() const;
    virtual unsigned int name9() const;
    virtual unsigned int name10() const;
    virtual unsigned int name11() const;
    virtual unsigned int name12() const;
    virtual unsigned int priority() const;
    virtual unsigned int ebins() const;
    virtual ACS* start_acs() const;
    virtual ACS* end_acs() const;
    virtual int gain() const;
    virtual int offset() const;
    virtual unsigned int block_ids() const;
    virtual unsigned int detectors() const;

    virtual void head_size(unsigned int value);
    virtual void ldp_type(unsigned int value);
    virtual void data_type(unsigned int value);
    virtual void name0(unsigned int value);
    virtual void name1(unsigned int value);
    virtual void name2(unsigned int value);
    virtual void name3(unsigned int value);
    virtual void name4(unsigned int value);
    virtual void name5(unsigned int value);
    virtual void name6(unsigned int value);
    virtual void name7(unsigned int value);
    virtual void name8(unsigned int value);
    virtual void name9(unsigned int value);
    virtual void name10(unsigned int value);
    virtual void name11(unsigned int value);
    virtual void name12(unsigned int value);
    virtual void priority(unsigned int value);
    virtual void ebins(unsigned int value);
    virtual void gain(int value);
    virtual void offset(int value);
    virtual void block_ids(unsigned int value);
    virtual void detectors(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(BATLDPHead& left,
                            BATLDPHead& right);

    friend int operator!=(BATLDPHead& left,                             BATLDPHead& right);

}; // end of BATLDPHead class
#endif // BATLDPHEAD_H
