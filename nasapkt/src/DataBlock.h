#ifndef DATA_BLOCK_H
#define DATA_BLOCK_H

#include "Reader.h"
#include "Writer.h"

/*****************************************************************************
* This is a generic class for describing a fixed size block of parsed binary
* data.
* This is an abstract class, which provides no real functionality of its own.
* Subclasses should provide data contents
* and methods for reading and writing.
* The classes produced by the db2class perl script are all subclasses of
* this one.
*****************************************************************************/
class DataBlock {

public:
    /*************
    * class data *
    *************/
    const static int size=0;

protected:
    /*********************************
    * data for string representation *
    *********************************/
    string line;

protected:
    /**************
    * constructor *
    **************/
    DataBlock() { };

    /**************
    * denstructor *
    **************/
    virtual ~DataBlock() {}



public:

    /*************************
    * purely virtual methods *
    *************************/
 //   virtual void read( Reader& r) throw(Interpreter::Exception);; 
 //   virtual void write(Writer& w) throw(Interpreter::Exception);;

    virtual void read( Reader* r) throw(Interpreter::Exception) = 0; 
    virtual void write(Writer* w) throw(Interpreter::Exception) = 0;

    /*********************************
    * create a string representation *
    *********************************/
    virtual const string& toString() {
        return line;
    }



}; // end of DataBlock class


#endif // DATA_BLOCK_H
