#ifndef RAW_DATA_BLOCK_H
#define RAW_DATA_BLOCK_H

#include "DataBlock.h"

class RawDataBlock : public DataBlock {

private:
    /*******
    * data *
    *******/
    int size;
    unsigned char* data;


public:
    /**************
    * constructor *
    **************/
    RawDataBlock(int size);
    RawDataBlock(unsigned char* data, int size);


    /*************
    * destructor *
    *************/
    virtual ~RawDataBlock() {delete data; }

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual void read( Reader* r) throw(Interpreter::Exception); 
    virtual void write(Writer* w) throw(Interpreter::Exception);


}; // end of RawDataBlock class

#endif // RAW_DATA_BLOCK_H
