#ifndef CCSDS_PACKET_H
#define CCSDS_PACKET_H

#include <cstdlib>
#include "DataBlock.h"
#include "CCSDSHead.h"

/*****************************************************************************
* This class represents a generic CCSDS telemetry packet.
* Such a packet consists of a 6 byte CCSDS primary header followed
* by some number of data bytes. 
*
* This class may be subclassed to handle secondary, etc. headers.
* A number of such sub-classes have been written for the Swift mission.
*****************************************************************************/
class CCSDSPacket : public CCSDSHead {

public:
    /*************
    * class data *
    *************/
    static const int SEQUENCE_PERIOD = 1<<14;
    static const int SEQUENCE_HALF_PERIOD = SEQUENCE_PERIOD>>1;

    static const int MAX_LENGTH = (2<<16) -1;

private:
    /*******
    * data *
    *******/
    unsigned char* _data;
    int array_size;

protected:
    /*******************
    * copy constructor *
    *******************/
    CCSDSPacket(const CCSDSPacket& orig);


public:
    /***************
    * constructors *
    ***************/
    CCSDSPacket();
    CCSDSPacket(int size);


    /*************
    * destructor *
    *************/
    virtual ~CCSDSPacket();


    /************
    * accessors *
    ************/
    virtual unsigned char* data() const { return _data; }

    virtual int  size() const; 
    virtual void size(int size); 

    virtual unsigned int  length() const;
    virtual void length(unsigned int value);

    virtual Reader* dataReader();
    virtual Writer* dataWriter();

    virtual int minimumLength();


    /**********
    * methods *
    **********/
    virtual void read(const CCSDSHead& head, Reader* r) 
                 throw(Interpreter::Exception);

    virtual void read( Reader* r) throw(Interpreter::Exception); 
    virtual void write(Writer* w) throw(Interpreter::Exception);

    virtual const string& toString();

    virtual CCSDSPacket* copy();

    virtual int samePlaceAs(CCSDSPacket* p);

    /************
    * operators *
    ************/
    friend int comes_before(CCSDSPacket* left, CCSDSPacket* right);
    friend int unique_comparator(CCSDSPacket* left, CCSDSPacket* right);
    virtual int operator<(const CCSDSPacket& right);
    virtual int isAdjacentTo(CCSDSPacket* p);

protected:
    /******************************************************************
    * secondary header and trailer methods for subclasses to override *
    ******************************************************************/
    virtual int extraHeadSize() const;
    virtual void  readExtraHeaders(Reader* r) throw(Interpreter::Exception);
    virtual void writeExtraHeaders(Writer* w) throw(Interpreter::Exception);

    virtual int trailerSize() const;
    virtual void  readTrailer(Reader* r) throw(Interpreter::Exception);
    virtual void writeTrailer(Writer* w) throw(Interpreter::Exception);

    virtual int extraSize() const;


protected:
    virtual void  initDataArray();
    virtual int   sizeAccordingToHeader() const;
    virtual void  sizeAccordingToHeader(int size);
    virtual void resizeDataArray(int size);

private:
    virtual void readBody(Reader* r) throw(Interpreter::Exception);


}; // end of CCSDSPacket class

#endif //  CCSDS_PACKET_H
