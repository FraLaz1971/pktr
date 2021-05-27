#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std;


/*****************************************************************************
* This is a generic base class for binary readers and writers.
* It provides support for a one byte buffer for handling individual bit I/O.
* It also has a number of I/O exception classes embedded in it.
*****************************************************************************/
class Interpreter {

protected:
    /*******
    * data *
    *******/
    unsigned char bit_buffer; // buffer for reading
    int n_bits_in_buffer;     // individual bits


    int bytes_per_int;
    int  bits_per_int;



public:
    /**************
    * constructor *
    **************/
    Interpreter() {

        n_bits_in_buffer=0;

        bytes_per_int = sizeof(int);
        bits_per_int = bytes_per_int*8;

    } // end of constructor

    /************
    * accessors *
    ************/
    virtual int leftOverBits() { return n_bits_in_buffer; }


    /*****************************
    * embedded exception classes *
    *****************************/

    /*********************************************************
    * generic interpreter exception 
    *********************************************************/
    class Exception : public runtime_error {

        public:
            Exception(const string& context) : runtime_error(context) { }


    }; // end of Exception class




    /********************************************************************
    * this exception indicates some error from the underlying IO stream
    ********************************************************************/
    class IOException : public Exception {

        public:
            IOException(const string& context) : Exception(context) {}
  
    }; // end of IOException embedded class

    /*************************************************************
    * this exception is thrown when the end of file is reached
    *************************************************************/
    class EOFException : public IOException {

        public:
            EOFException(const string& context): IOException(context) { }

    }; 



    /***********************************************************
    * exception indicating an incompatibility between the
    * machine representation of variables and this class
    ***********************************************************/
    class HardwareException : public Exception {
    
        public:
            HardwareException(const string& context): Exception(context) { }

    }; // end of HardwareException embedded class


protected:

    /*****************
    * utility method *
    *****************/
    virtual void checkStream(ios* stream, const string& context) 
                 throw(IOException);

public:
    /*****************
    * static methods *
    *****************/
    static void checkIEEE() throw(HardwareException);
    static double powerOf2(int power);
    static int logBase2(double value, double* remainder=NULL);

};


#endif //  INTERPRETER_H
