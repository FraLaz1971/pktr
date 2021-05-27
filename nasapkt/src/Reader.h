#ifndef READER_H
#define READER_H

#include <iostream>
//#include <stdexcept>
#include "Interpreter.h"

/****************************************************************************
* This class wraps around an istream and provides a machine-independant 
* framework for reading various binary encoded data types.
* Sub-classes of this class may be optimized for individual platforms.
****************************************************************************/
class Reader : public Interpreter {

private:
    /*******
    * data *
    *******/
    istream* in;  // underlying IO stream

public:
    /**************
    * constructor *
    **************/
    Reader(istream& in);
    Reader(istream* in);

    /*************
    * destructor *
    *************/
    virtual ~Reader();

    /************
    * accessors *
    ************/
    virtual istream* getStream();

    /**********
    * methods *
    **********/
    virtual unsigned char readByte()                         throw(Exception);
    virtual int readBytes(unsigned char* buffer, int n)      throw(Exception);
    virtual int readBytes(unsigned char* buffer, int n, int min) 
                                                             throw(Exception);

    virtual int readInt(int bytes)                           throw(Exception) ;
    virtual unsigned int readUnsignedInt(int bytes)          throw(Exception);

    virtual unsigned int readUnsignedBits(int n_bits_needed) throw(Exception);
    virtual int readSignedBits(int n_bits_needed)            throw(Exception);

    virtual float readFloat()                                throw(Exception);
    virtual double readDouble()                              throw(Exception);

    /*****************
    * static methods *
    *****************/
    static void checkMachine() throw(HardwareException) ;
    virtual void readTestPattern() throw(Exception);


};

#endif // READER_H
