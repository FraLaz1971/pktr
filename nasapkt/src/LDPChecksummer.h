#ifndef LDPCHECKSUMMER_H
#define LDPCHECKSUMMER_H

#include "Checksummer.h"
#include <iostream>

/****************************************************************************
* This class calculates checksums for Swift Large Data Product classes.
* It simply calculates the sum of all bytes modulo 2^16.
* See the Checksummer for more details on usage.
****************************************************************************/
class LDPChecksummer : public Checksummer {

private:
    /*******
    * data *
    *******/
    unsigned int sum;
    unsigned int modulus;


public:
    /**************
    * constructor *
    **************/
    LDPChecksummer();

    /*************
    * destructor *
    *************/
    virtual ~LDPChecksummer() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual unsigned long value();
    virtual void reset() { sum=0; }

    virtual void addToSum(unsigned char c);


}; // end of LDPChecksummer class

#endif // LDPCHECKSUMMER_H
