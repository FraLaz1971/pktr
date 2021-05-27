#include "Reader.h"

/****************************************************************************
* constructor from istream passed by reference
****************************************************************************/
Reader::Reader(istream& in) : Interpreter() {

this->in = &in;


} // end of constructor

/**************************************************************************
* constructor from istream passed as a pointer 
**************************************************************************/
Reader::Reader(istream* in) : Interpreter() {

this->in = in;


} // end of constructor


/*************
* destructor *
*************/
Reader::~Reader() {

//in->close();

} // end of destructor

/***************************************************************************
* returns a pointer to the underlying istream
***************************************************************************/
istream* Reader::getStream() { return in; }

/***************************************************************************
* read a single byte
***************************************************************************/
unsigned char Reader::readByte() throw(Exception){

//cout << "    reading byte\n";

n_bits_in_buffer=0; // empty the bit buffer

unsigned char byte;

//in->get(byte); changed on Rich Fink's suggestion 2002-05-10
in->read((char*)&byte,1);
checkStream(in,"reading byte");

return byte;

} // end of readByte method

/***************************************************************************
* read a given number of bytes into an array. 
* Returns the number of bytes actually read
* Note this method will throw an IOException (e.g. EOFException)
* only if no bytes can be read from the underlying stream
***************************************************************************/
int Reader::readBytes(unsigned char* buffer, int n) throw(Exception) {

return readBytes(buffer,n,1);

} // end of readBytes method


/***************************************************************************
* read a given number of bytes into an array. 
* Returns the number of bytes actually read
* This method will throw an IOException (e.g. EOFException)
* if fewer than "min" bytes were read from the underlying stream
***************************************************************************/
int Reader::readBytes(unsigned char* buffer, int n, int min) throw(Exception) {

//cout << "Reader::readBytes start\n";
//cout << "Reader::readBytes n="<<n<<"\n";

//cout << "readBytes n="<<n<<" min="<<min<<"\n";


in->read((char*)buffer,n);

int nread = in->gcount();
//cout << "actually got "<<nread<<" bytes\n";
if(nread<min) checkStream(in, "while reading a block of bytes");

return nread;

} // end of readBytes method


/***************************************************************************
* read an unsigned integer of a given number of bytes.
* The most significant bytes are read first.
***************************************************************************/
unsigned int Reader::readUnsignedInt(int bytes) throw(Exception) {

// cout << "Reader::readUnsignedInt start\n";

if(bytes>bytes_per_int) {
    throw HardwareException("int won't hold requested bytes");
} 

unsigned int value=0;
for(int i=0; i<bytes; ++i ) {
    value <<= 8;
    value |= readByte();
}

return value;

} // end of readUnsignedInt method

/***************************************************************************
* read a twos compliment signed integer of a a given number of bytes.
* The most significant bytes are read first.
***************************************************************************/
int Reader::readInt(int bytes) throw(Exception) {

unsigned int uvalue = readUnsignedInt(bytes);

unsigned int max = 1 << (bytes*8-1);

//cout << "readInt: uvalue="<<uvalue << " "
//     << "max="<<max
//     << "\n";

int value;
if(uvalue > max -1 ) value = (int)(uvalue - (max<<1) );
else value = uvalue;

return value;

} // end of readUnsignedInt method



/***************************************************************************
* read a given number of bits as an unsigned integer
***************************************************************************/
unsigned int Reader::readUnsignedBits(int n_bits_needed) throw(Exception) {


if(n_bits_needed > bits_per_int) {
    throw HardwareException("int won't hold requested bits");
} 


unsigned int value=0; // initialize return value to zero

/**********************************************************************
* take bits out of the buffer until we have read all the bits we need *
**********************************************************************/
for(int n_bits_to_read=0 ; n_bits_needed; n_bits_needed -= n_bits_to_read ) {

     //cout << "readUnsignedBits: to read="<< n_bits_to_read
     //    << " n_bits_in_buffer=" << n_bits_in_buffer
     //    << "\n";


    /***********************************
    * refill the buffer if it is empty *
    ***********************************/
    if(! n_bits_in_buffer) {
        bit_buffer=readByte();
        n_bits_in_buffer = 8;
    }
    

    /**************************************************************
    * determine how many bits we can/need to take from the buffer *
    **************************************************************/
    n_bits_to_read = n_bits_needed;
    if(n_bits_to_read > n_bits_in_buffer) n_bits_to_read = n_bits_in_buffer;

    /**********************************
    * take the bits out of the buffer *
    **********************************/
    value |= (bit_buffer >> (8- n_bits_to_read))  // clear out unwanted bits
             << (n_bits_needed - n_bits_to_read); // shift the bits into place

    bit_buffer <<= n_bits_to_read;  // keep the bit buffer left justified
    n_bits_in_buffer -= n_bits_to_read;

    // cout << "readUnsignedBits: to read at end of loop="<< n_bits_to_read
    //     << "\n";


} // end of loop taking bits from the buffer

return value;

} // end of readUnsignedBits method

/***************************************************************************
* read a given number of bits as a twos compliment signed integer
***************************************************************************/
int Reader::readSignedBits(int n_bits_needed) throw(Exception) {

unsigned int uvalue = readUnsignedBits(n_bits_needed);

unsigned int max = 1 << (n_bits_needed-1);

int value;
if(uvalue > max -1 ) value = (int)(uvalue - (max<<1) );
else value = uvalue;

return value;


} // end of readSignedBits method

/***************************************************************************
* Read a 4 byte IEEE-754 format floating point number
***************************************************************************/
float Reader::readFloat() throw(Exception) {

float value;

int sign = 1 - 2* readUnsignedBits(1);
unsigned int exponent = readUnsignedBits(8);
float fraction = ((float)readUnsignedBits(23)/(float)(1<<23));

/*
cout << "sign="<<sign
     << " exponent="<<exponent
     << " fraction="<<fraction
     <<"\n";
*/

float zero=0.0; // this is to shut the compiler up

if(exponent==255) {
    if(fraction!=0) value = 0./zero;
    if(fraction==0) value = sign/zero;

} else if(exponent==0) {
    if(fraction==0 ) value = sign * 0.0;
    else   value = sign * fraction * powerOf2(-126);

} else {

    value = sign * (1+fraction) * powerOf2(exponent-127);

}
        

return value;

} // end of readFloat method

/***************************************************************************
* Read a 8 byte IEEE-754 format floating point number
***************************************************************************/
double Reader::readDouble() throw(Exception) {

double value;

int sign = 1 - 2* readUnsignedBits(1);
unsigned int exponent = readUnsignedBits(11);
double fraction = ((double)readUnsignedBits(32)/powerOf2(32));
fraction += ((double)readUnsignedBits(20)/powerOf2(52));


/*
cout << "sign="<<sign
     << " exponent="<<exponent
     << " fraction="<<fraction
     <<"\n";
*/

double zero=0.0; // this is to shut the compiler up

if(exponent==2047) {
    if(fraction!=0) value = 0./zero;
    if(fraction==0) value = sign/zero;

} else if(exponent==0) {
    if(fraction==0 ) value = sign * 0.0;
    else   value = sign * fraction * powerOf2(-1022);

} else {

    value = sign * (1+fraction) * powerOf2(exponent-1023);

}

return value;

} // end of readFloat method




/***************************************************************************
* Test whether the machine running this code stores variables in a way
* consistent with the assumptions made when writing this class.
* This particular class makes no assumptions, so this method does nothing.
***************************************************************************/
void Reader::checkMachine() throw(HardwareException) {

} // end of checkMachine static method


/***************************************************************************
* read the set of values written by Writer::writeTestPattern. Throws a
* HardwareException if there was an inconsistency.
***************************************************************************/
void Reader::readTestPattern() throw(Exception) {

double zero=0.;

/**************
* IEEE floats *
**************/
{
float value;

if(readFloat() != 1./zero) {
    throw HardwareException("4 byte float +Inf not read correctly");
}

if(readFloat() != -1./zero) {
    throw HardwareException("4 byte float -Inf not read correctly");
}

value=readFloat();
if(value <=0. || value >0.) {
    throw HardwareException("4 byte float NaN not read correctly");
}

value=readFloat();
if(value !=0. || 1./value <0. ) {
    throw HardwareException("4 byte float positive zero not read correctly");
}

value=readFloat();
if(value !=-0. || 1./value >0. ) {
    throw HardwareException("4 byte float negative zero not read correctly");
}


value=readFloat();
if(value !=1. ) {
    throw HardwareException("4 byte float 1. not read correctly");
}

value=readFloat();
if(value !=(float)2.1 ) {
    throw HardwareException("4 byte float 2.1 not read correctly");
}

value=readFloat();
if(value !=(float)0.4 ) {
    throw HardwareException("4 byte float 0.4 not read correctly");
}


value=readFloat();
if(value !=(float)-1.3e30 ) {
    throw HardwareException("4 byte float -1.3e30 not read correctly");
}




value=readFloat();
if(value !=(float)1.2e-40 ) {
    throw HardwareException("4 byte float unnormalized value 1.2e-40 not read correctly");
}

value=readFloat();
if(value !=(float)-6.7e-41 ) {
    throw HardwareException("4 byte float unnormalized value -6.7e-41 not read correctly");
}

} // end of IEEE floats

/***************
* IEEE doubles *
***************/
{
double value;

value = readDouble();
if(value != 1./zero) {
    throw HardwareException("8 byte float +Inf not read correctly");
}

value=readDouble();
if(value != -1./zero) {
    throw HardwareException("8 byte float -Inf not read correctly");
}

value=readDouble();
if(value <=0. || value >0.) {
    throw HardwareException("8 byte float NaN not read correctly");
}

value=readDouble();
if(value !=0. || 1./value <0. ) {
    throw HardwareException("8 byte float positive zero not read correctly");
}

value=readDouble();
if(value !=-0. || 1./value >0. ) {
    throw HardwareException("8 byte float negative zero not read correctly");
}


value=readDouble();
if(value !=1. ) {
    throw HardwareException("8 byte float 1. not read correctly");
}

value=readDouble();
if(value !=2.1 ) {
    throw HardwareException("8 byte float 2.1 not read correctly");
}

value=readDouble();
if(value !=0.4 ) {
    throw HardwareException("8 byte float 0.4 not read correctly");
}


value=readDouble();
if(value !=-1.3e30 ) {
    throw HardwareException("8 byte float -1.3e30 not read correctly");
}


value=readDouble();
if(value !=1.2e-40 ) {
    throw HardwareException("8 byte float unnormalized value 1.2e-40 not read correctly");
}

value=readDouble();
if(value !=-6.7e-41 ) {
    throw HardwareException("8 byte float unnormalized value -6.7e-41 not read correctly");
}



} // done with doubles


/***********
* integers *
***********/
if(readUnsignedBits(1) != 1 ) {
    throw HardwareException("1 bit integer read incorrectly");
}

if(readUnsignedBits(10) != 916 ) {
    throw HardwareException("10 bit unsigned integer read incorrectly");
}

if(readSignedBits(5) != -14 ) {
    throw HardwareException("5 bit Signed integer read incorrectly");
}

if((readInt(3)) != -5000000 ) {
    throw HardwareException("3 byte signed integer read incorrectly");
}

if(readByte() != 137 ) {
    throw HardwareException("single byte read incorrectly");
}


if((readInt(4)) != -7 ) {
    throw HardwareException("4 byte signed integer read incorrectly");
}


if((readUnsignedInt(2)) != 50000 ) {
    throw HardwareException("2 byte unsigned integer read incorrectly");
}



unsigned char bytes[2];

readBytes(bytes, 2);

if(bytes[0] != 114 || bytes[1] != 12) {
    throw HardwareException("Couldn't read a set of two bytes correctly");
}

    
try {
readByte();
} catch(EOFException) {
    return;
} 

throw HardwareException("Extra bytes after test pattern");


    

} // end of readTestPattern method
