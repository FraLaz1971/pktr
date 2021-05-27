#ifndef WRITER_H
#define WRITER_H

#include "Interpreter.h"

/****************************************************************************
* This class wraps around an ostream and provides a machine-independant 
* framework for writing various binary encoded data types.
* Sub-classes may be optimized for individual platforms.
****************************************************************************/
class Writer : public Interpreter {

private:
    /*******
    * data *
    *******/
    ostream* out; // underlying IO stream

public:
    /**************
    * constructor *
    **************/
    Writer(ostream& out);
    Writer(ostream* out);

    /*************
    * destructor *
    *************/
    virtual ~Writer();

    /************
    * accessors *
    ************/
    virtual ostream* getStream();



    /**********
    * methods *
    **********/
    virtual void writeByte(unsigned char value)                throw(Exception);
    virtual void writeBytes(unsigned char* buffer, int n)      throw(Exception);

    virtual void writeInt(int value, int bytes)               throw(Exception) ;
    virtual void writeUnsignedInt(unsigned int value, int bytes) throw(Exception);

    virtual void writeUnsignedBits(unsigned int value, int n) throw(Exception);
    virtual void writeSignedBits(int value, int n) throw(Exception);

    virtual void writeFloat(float value)                       throw(Exception);
    virtual void writeDouble(double value)                     throw(Exception);

    /*****************
    * static methods *
    *****************/
    static void checkMachine() throw(HardwareException);
    virtual void writeTestPattern() throw(Exception);



 };


#endif // WRITER_H
