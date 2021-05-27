#include "RawDataBlock.h"

/*********************************************************************
* constructor
*********************************************************************/
RawDataBlock::RawDataBlock(int size) {

    this->size = size;
    data = new unsigned char[size];

} // end of constructor

/*********************************************************************
* constructor
*********************************************************************/
RawDataBlock::RawDataBlock(unsigned char* data, int size) {

    this->size = size;
    this->data = data;

} // end of constructor


/*********************************************************************
*
*********************************************************************/
void RawDataBlock::read(Reader* r) throw(Interpreter::Exception) {

    r->readBytes(data, size, size);
}


/*********************************************************************
*
*********************************************************************/
void RawDataBlock::write(Writer* w) throw(Interpreter::Exception) {

    w->writeBytes(data, size);
}
