// This file was automatically generated from ITOSAnnotation.db.
// Do not Edit! Instead, create a subclass to extend functionality

#ifndef ITOSANNOTATION_H
#define ITOSANNOTATION_H

#include "DataBlock.h"

/*************************************************************
* This class was machine generated from the file ITOSAnnotation.db
* by the perl script db2class.
*
* The following comments have been copied from that file:
*
* This is an ITOS-like database definition for 
* the 12 byte ITOS packet annotation header.
*************************************************************/
class ITOSAnnotation : public DataBlock {

public:
    const static int size=12;

private:
    unsigned int _version; // Version field from transfer frame header
    unsigned int _spacecraft; // Spacecraft ID from transfer frame header
    unsigned int _vc; // Virtual Channel from transfer frame
    unsigned int _reserved; // this bit not used
    unsigned int _rs_enabled; // Reed Solomon enabled if set
    unsigned int _rs_error; // uncorrectable Reed Solomon error if set
    unsigned int _rs_corrected; // Reed Solomon error corrected if set
    unsigned int _time_format; // Time code format 0=none, 1=PB1, 4=PB4, 8 =relative TIME42, 9=absolute TIME42
    unsigned int _head_error; // packet header from bad frame if set
    unsigned int _reversed; // data received in rever bit order if set
    unsigned int _seq_error; // Sequence gap from previous packet if set
    unsigned int _frame_error; // uncorrected error in parent frame if set
    unsigned int _checking; // frame error checking enabled if set
    unsigned int _incomplete; // packet incomplete (filled) if set
    unsigned int _vc_seq_error; // transfer frame gap from previous if set
    unsigned int _frame_head_error; // incorrect version or s/c ID in frame header if set
    unsigned int _fill; // fill data start byte offset from end of primary header
    unsigned int _time; // ground received time from frame header
    unsigned int _time2; // ground received time from frame header contd

public:
    ITOSAnnotation() : DataBlock() {
    } // end of constructor

    ITOSAnnotation(const ITOSAnnotation& orig) : DataBlock() {
        _version =  orig.version();
        _spacecraft =  orig.spacecraft();
        _vc =  orig.vc();
        _reserved =  orig.reserved();
        _rs_enabled =  orig.rs_enabled();
        _rs_error =  orig.rs_error();
        _rs_corrected =  orig.rs_corrected();
        _time_format =  orig.time_format();
        _head_error =  orig.head_error();
        _reversed =  orig.reversed();
        _seq_error =  orig.seq_error();
        _frame_error =  orig.frame_error();
        _checking =  orig.checking();
        _incomplete =  orig.incomplete();
        _vc_seq_error =  orig.vc_seq_error();
        _frame_head_error =  orig.frame_head_error();
        _fill =  orig.fill();
        _time =  orig.time();
        _time2 =  orig.time2();
    } // end of copy constructor

    virtual unsigned int version() const;
    virtual unsigned int spacecraft() const;
    virtual unsigned int vc() const;
    virtual unsigned int reserved() const;
    virtual unsigned int rs_enabled() const;
    virtual unsigned int rs_error() const;
    virtual unsigned int rs_corrected() const;
    virtual unsigned int time_format() const;
    virtual unsigned int head_error() const;
    virtual unsigned int reversed() const;
    virtual unsigned int seq_error() const;
    virtual unsigned int frame_error() const;
    virtual unsigned int checking() const;
    virtual unsigned int incomplete() const;
    virtual unsigned int vc_seq_error() const;
    virtual unsigned int frame_head_error() const;
    virtual unsigned int fill() const;
    virtual unsigned int time() const;
    virtual unsigned int time2() const;

    virtual void version(unsigned int value);
    virtual void spacecraft(unsigned int value);
    virtual void vc(unsigned int value);
    virtual void reserved(unsigned int value);
    virtual void rs_enabled(unsigned int value);
    virtual void rs_error(unsigned int value);
    virtual void rs_corrected(unsigned int value);
    virtual void time_format(unsigned int value);
    virtual void head_error(unsigned int value);
    virtual void reversed(unsigned int value);
    virtual void seq_error(unsigned int value);
    virtual void frame_error(unsigned int value);
    virtual void checking(unsigned int value);
    virtual void incomplete(unsigned int value);
    virtual void vc_seq_error(unsigned int value);
    virtual void frame_head_error(unsigned int value);
    virtual void fill(unsigned int value);
    virtual void time(unsigned int value);
    virtual void time2(unsigned int value);

    virtual void read(Reader* r) throw(Interpreter::Exception);
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    friend int operator==(ITOSAnnotation& left,
                            ITOSAnnotation& right);

    friend int operator!=(ITOSAnnotation& left,                             ITOSAnnotation& right);

}; // end of ITOSAnnotation class
#endif // ITOSANNOTATION_H
