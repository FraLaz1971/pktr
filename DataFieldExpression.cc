#include "DataFieldExpression.h"

#include <sstream>
#include "DataBuf.h"
#include "Reader.h"

/*********************************************************************
* constructor
*********************************************************************/
DataFieldExpression::DataFieldExpression(char type, 
                                         int bytes, int bits, int size) {

    ostringstream format;

    format << type << "["<<bytes<<"."<<bits<<", "<<size<<"]";
    name = format.str();

    this->type = type;
    this->bytes = bytes;
    this->bits = bits;
    this->size = size;

    bytes += bits/8;
    bits   = bits %8;

} // end of constructor

/*********************************************************************
*
*********************************************************************/
double DataFieldExpression::evaluate(Context* c) throw(Exception) {

    CCSDSPacket* p = c->packet();
    if(p==NULL) throw Exception("Null packet\n");

    if(p->size() < bytes + (bits+size+7)/8 ) {
        throw Exception("packet does not contain %"+name);
    }

    /**********************
    * get the data reader *
    **********************/
    istream* in = new istream(new DataBuf((char*)p->data()+bytes, 
                                                 p->size()-bytes));
    Reader* r = new Reader(in);

    if(bits) r->readUnsignedBits(bits);


    /****************
    * read the data *
    ****************/
    if(     type == 'U') return (double)r->readUnsignedBits(size);
    else if(type == 'I') return (double)r->readSignedBits(size);
    else if(type == 'F' && size == 32) return (double)r->readFloat();
    else if(type == 'F' && size == 64) return (double)r->readDouble();
    else throw Exception("Invalid data field %"+name);

    delete in;
    delete r;
        

} // end of evaluate method


/*********************************************************************
*
*********************************************************************/
void DataFieldExpression::set(Context* c, double value) throw(Exception) {

    CCSDSPacket* p = c->packet();
    if(p==NULL) throw Exception("Null packet\n");

    if(p->size() < bytes + (bits+size+7)/8 ) {
        throw Exception("packet does not contain %"+name);
    }

//cerr << "Writing to "<<this<<"\n";

    /**********************
    * get the data writer *
    **********************/
    ostream* out = new ostream(new DataBuf((char*)p->data()+bytes, 
                                                  p->size()-bytes));
    Writer* w = new Writer(out);

    /****************
    * skip odd bits *
    ****************/
    if(bits) {
        unsigned int value = p->data()[bytes];
        value >>= (8-bits);
        w->writeUnsignedBits(value, bits);
    }

    /*****************
    * write the data *
    *****************/
    if(     type == 'U') w->writeUnsignedBits((unsigned int)value, size);
    else if(type == 'I') w->writeSignedBits((int)value, size);
    else if(type == 'F' && size == 32) w->writeFloat((float)value);
    else if(type == 'F' && size == 64) w->writeDouble(value);
    else throw Exception("Invalid data field %"+name);

    delete w;
    delete out;

} // end of set method
