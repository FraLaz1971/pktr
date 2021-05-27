#ifndef CHECKSUMMER_H
#define CHECKSUMMER_H

//#include <streambuf.h>
#include <cstdio>
#include <iostream>

/*****************************************************************************
* This is an abstract base class for classes which calculate checksums
* and CRCs for a data stream. It is implemented as a streambuf. So the
* way to use it is to create a Checksummer, wrap it in an ostream,
* and then write your data to that ostream.
* The value() method will return the value of the checksum, and reset()
* will reset the value of the checksum as if no data had been read.
*****************************************************************************/
class Checksummer : public std::streambuf {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    Checksummer();

    /*************
    * destructor *
    *************/
    virtual ~Checksummer() {}

    /************
    * accessors *
    ************/
    

    /**********
    * methods *
    **********/
    virtual unsigned long value()=0;
    virtual void reset()=0;

    virtual void addToSum(unsigned char c) =0;

    /*****************************
    * streambuf override methods *
    *****************************/
    virtual int overflow(int c = EOF); 


}; // end of Checksummer class

#endif // CHECKSUMMER_H
