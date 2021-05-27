#include "Writer.h"

/*************************************************************************
* constructor from ostream passed by reference
*************************************************************************/
Writer::Writer(ostream& out) : Interpreter() {

this->out = &out;


} // end of constructor

/***************************************************************************
* constructor from ostream passed as a pointer 
***************************************************************************/
Writer::Writer(ostream* out) : Interpreter() {

this->out = out;


} // end of constructor



/******************************************************************************
* destructor. This method flushes the ostream, but does not close or delete it
******************************************************************************/
Writer::~Writer() {

//cout << "destructing writer about to flush stream\n";

out->flush();

} // end of destructor


/******************************************************************************
* return a pointer to the underlying ostream
******************************************************************************/
ostream* Writer::getStream() { return out; }


/*****************************************************************************
* write a single byte 
*****************************************************************************/
void Writer::writeByte(unsigned char value) throw(Exception) {


out->write((char*)&value, 1);
checkStream(out, "while writing a byte");

} // end of writeByte method

/*****************************************************************************
* write a given number of bytes 
*****************************************************************************/
void Writer::writeBytes(unsigned char* buffer, int n) throw(Exception) {

out->write((char*)buffer,n);
checkStream(out, "writing a block of bytes" );

} // end of writeBytes method


/*****************************************************************************
* write an unsigned big-endian integer a given number of bytes in length 
*****************************************************************************/
void Writer::writeUnsignedInt(unsigned int value, int bytes) throw(Exception) {

if(bytes > bytes_per_int) {
    throw HardwareException("int doesn't hold requested bytes");
}


/******************
* loop over bytes *
******************/
for(int i=0; i<bytes; ++i) {

    unsigned char byte = (value >> ((bytes-i-1)*8)) & 255;
    writeByte(byte);

} // end of loop over bytes


} // end of writeUnsignedInt method

/*****************************************************************************
* write an twos complement, big-endian integer a given number of bytes 
* in length 
*****************************************************************************/
void Writer::writeInt(int value, int bytes) throw(Exception) {

/********************************************
* convert to an unsigned integer which 
* holds the twos compliment representation
********************************************/
unsigned int all_fs = (1<< (bytes*8-1)) -1 ;
all_fs <<= 1;
all_fs |= 1;

unsigned int uvalue;
if(value <0) uvalue = (value + all_fs)+1;
else         uvalue = value;


writeUnsignedInt(uvalue,bytes);


} // end of writeInt method

/*****************************************************************************
* write a given unsigned integer represented by a given number of bits.
*****************************************************************************/
void Writer::writeUnsignedBits(unsigned int value, int n) throw(Exception) {

//cout << "Writer::writeUnsignedBits\n";

if(n > bits_per_int) {
    throw HardwareException("int doesn't hold requested number of bits");
} 

int shift = n-1;

for(int i=0; i<n; ++i) {

//cout << i 
//     << " n_bits_in_buffer="<< n_bits_in_buffer << "\n";

    /*************************************************
    * add the most significant bit to the bit buffer *
    *************************************************/
    bit_buffer <<= 1;
    bit_buffer |= ((value >> shift ) & 1);
    ++n_bits_in_buffer;

    value <<= 1;

    /**********************************
    * do we need to flush the buffer? *
    **********************************/
    if(n_bits_in_buffer == 8) {
//cout << "flushing bit buffer\n";
        //out->put(bit_buffer);
        out->write((char*)&bit_buffer, 1);
        checkStream(out,"while flushing the bit buffer");

        bit_buffer=0;
        n_bits_in_buffer=0;
    }

} // end of loop over bits

} // end of writeUnsignedBits method
    

/*****************************************************************************
* write a given twos compliment signed integer represented by a given 
* number of bits 
*****************************************************************************/
void Writer::writeSignedBits(int value, int n) throw(Exception) {

/********************************************
* convert to an unsigned integer which 
* holds the twos compliment representation
********************************************/

unsigned int uvalue;

if(value <0) uvalue = value + (unsigned int)(1 << n ) ;
else         uvalue = value;

/*
cout << "uvalue=" << uvalue 
     << "value="  << value << "\n";
*/

writeUnsignedBits(uvalue,n);


} // end of writeInt method

/***************************************************************************
* Write a 4 byte IEEE format floating point number
***************************************************************************/
void Writer::writeFloat(float value) throw(Exception) {

if( !(value >=0. || value <0.) ) {
    /******************************
    * value is NaN - not a number *
    ******************************/

    writeUnsignedBits(0, 1);
    writeUnsignedBits(255, 8);
    writeUnsignedBits(1, 23);


} else if(1./value == 0.) {
    /***********
    * infinity *
    ***********/
    unsigned int sign=0;
    if(value<0.) sign=1;

    writeUnsignedBits(sign, 1);
    writeUnsignedBits(255, 8);
    writeUnsignedBits(0, 23);

} else if(value == 0.) {
    /*******
    * zero *
    *******/
    if(1./value >0.) writeUnsignedBits(0,32);
    else {
        /****************
        * negative zero *
        ****************/
        writeUnsignedBits(1,1);
        writeUnsignedBits(0,31);
    }


} else {
    /*****************
    * regular number *
    *****************/
    unsigned int sign=0;
    if(value<0) {
        sign=1;
        value=-value;
    }

    double remainder;
    int exponent = logBase2(value, &remainder);
    unsigned int fraction = (unsigned int)(remainder*powerOf2(23));

/*
    cout << "exponent="<<exponent
         <<hex<<" fraction="<<fraction<<dec
         <<" sign="<< sign
         <<"\n";
*/


    if(exponent < -126) {
        /*********************
        * unnormalized value *
        *********************/
        writeUnsignedBits(sign, 1);
        writeUnsignedBits(0, 8);
        writeUnsignedBits((unsigned int)(fraction*powerOf2(exponent+126)), 23);

     
//cout << "unnormalized\n";
    } else {
        /*******************
        * normalized value *
        *******************/
//cout << "wrote value\n";
        writeUnsignedBits(sign, 1);
        writeUnsignedBits((unsigned int)(exponent+127), 8);
        writeUnsignedBits(fraction-8388608, 23);

    }


    
}
    
} // end of writeFloat method

/***************************************************************************
* Write a 8 byte IEEE format floating point number
***************************************************************************/
void Writer::writeDouble(double value) throw(Exception) {

if( !(value >=0. || value <0.) ) {
    /******************************
    * value is NaN - not a number *
    ******************************/
    writeUnsignedBits(0, 1);
    writeUnsignedBits(2047, 11);
    writeUnsignedBits(1, 32);
    writeUnsignedBits(0, 20);


} else if(1./value == 0.) {
    /***********
    * infinity *
    ***********/
    unsigned int sign=0;
    if(value<0.) sign=1;

    writeUnsignedBits(sign, 1);
    writeUnsignedBits(2047, 11);
    writeUnsignedBits(0, 32);
    writeUnsignedBits(0, 20);


} else if(value == 0.) {
    /*******
    * zero *
    *******/
    if(1./value >0.) {
        writeUnsignedBits(0,32);
        writeUnsignedBits(0,32);
    } else {
        /****************
        * negative zero *
        ****************/
        writeUnsignedBits(1,1);
        writeUnsignedBits(0,31);
        writeUnsignedBits(0,32);
    }


} else {
    /*****************
    * regular number *
    *****************/
    unsigned int sign=0;
    if(value<0) {
        sign=1;
        value=-value;
    }

    double remainder;
    int exponent = logBase2(value, &remainder);

       
/*                

    cout << "exponent="<<exponent
         << "remainder="<<remainder
         <<" sign="<< sign
         <<"\n";
*/


    if(exponent < -1022) {
        /*********************
        * unnormalized value *
        *********************/
        writeUnsignedBits(sign, 1);
        writeUnsignedBits(0, 11);

        remainder *= powerOf2(exponent+1022);

        unsigned int fraction1 = (unsigned int)(remainder*powerOf2(26));
        unsigned int fraction2 = (unsigned int)(
                           (remainder*powerOf2(26)-fraction1)*powerOf2(26));

        writeUnsignedBits(fraction1, 26);
        writeUnsignedBits(fraction2, 26);

     
//cout << "unnormalized\n";
    } else {
        /*******************
        * normalized value *
        *******************/
//cout << "wrote value\n";
        writeUnsignedBits(sign, 1);
        writeUnsignedBits((unsigned int)(exponent+1023), 11);

        unsigned int fraction1 = (unsigned int)(remainder*powerOf2(26));
        unsigned int fraction2 = (unsigned int)(
                           (remainder*powerOf2(26)-fraction1)*powerOf2(26));

/*
cout <<" fraction1="<<fraction1
     <<" fraction2="<<fraction2
     <<"\n";
*/


        writeUnsignedBits(fraction1-67108864, 26);
        writeUnsignedBits(fraction2, 26);

    }


    
}


} // end of writeDouble method

/***************************************************************************
* Test whether the machine running this code stores variables in a way
* consistent with the assumptions made when writing this class.
* This particular makes no assumptions so this method does nothing.
***************************************************************************/
void Writer::checkMachine() throw(HardwareException) {


} // end of checkMachine static method



/***************************************************************************
* Write a fixed set of values. These can then be read back with 
* the readTestPattern method to check a reader and writer for self-consistency
***************************************************************************/
void Writer::writeTestPattern() throw(Exception) {

double zero=0.;

/**************
* IEEE floats *
**************/
writeFloat(1./zero);
writeFloat(-1./zero);
writeFloat(zero/zero);
writeFloat(0.);
writeFloat(-0.);
writeFloat(1.);
writeFloat(2.1);
writeFloat(.4);
writeFloat(-1.3e30);
writeFloat(1.2e-40);
writeFloat(-6.7e-41);

/***************
* IEEE doubles *
***************/
writeDouble(1./zero);
writeDouble(-1./zero);
writeDouble(zero/zero);
writeDouble(0.);
writeDouble(-0.);
writeDouble(1.);
writeDouble(2.1);
writeDouble(.4);
writeDouble(-1.3e30);
writeDouble(1.2e-40);
writeDouble(-6.7e-41);


/***********
* integers *
***********/
writeUnsignedBits(1,1);
writeUnsignedBits(916,10);
writeSignedBits(-14, 5);

writeInt(-5000000, 3);
writeByte(137);
writeInt(-7, 4);

writeUnsignedInt(50000, 2);

unsigned char bytes[2];
bytes[0] = 114;
bytes[1] = 12;
writeBytes(bytes,2);

} // end of writeTestPattern method

