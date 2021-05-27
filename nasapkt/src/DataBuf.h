#ifndef DATA_BUF_H
#define DATA_BUF_H

#include <iostream> // we really want streambuf but this is backwards compat.

/*****************************************************************************
* This class implements binary I/O from memory. This streambuf subclass
* can be used for either data input or output with separate "current position"
* pointers for each direction.
* Now that strstream is deprecated in standard C++ I don't know of any
* other good way to do this.
*****************************************************************************/

class DataBuf : public std::streambuf {

private:
    /*******
    * data *
    *******/


public:
    /**************
    * constructor *
    **************/
    DataBuf(char* buffer, int size);

    /*************
    * destructor *
    *************/
    virtual ~DataBuf() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/

}; // end of DataBuf class

#endif // DATA_BUF_H
