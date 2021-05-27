#include "Interpreter.h"

/****************************************************************************
* Check if this machine uses the stardard 4 and 8 byte IEEE representation for
* floats and doubles, respectively.
* Throws a HardwareException if the machine is not compliant.
****************************************************************************/
void Interpreter::checkIEEE() throw(HardwareException) {

/*******************************************
* check that floats are 4 byte IEEE format *
*******************************************/
if(sizeof(float) != 4) {
    throw HardwareException("float is not 4 bytes long");
}

float f=1.23456789123456789123456789e12;
unsigned char* c = (unsigned char*)(&f);

if(! (c[0] ==  83 && 
      c[1] == 143 && 
      c[2] == 184 && 
      c[3] == 254)   ) {
    /*******************
    * not in IEEE format *
    *********************/
    throw HardwareException("float is not IEEE format");
}

/*******************************************
* check that doubles are 8 byte IEEE format *
*******************************************/
if(sizeof(double) != 8) {
    throw HardwareException("double is not 8 bytes long");
}

double d=1.23456789123456789123456789e123;
c = (unsigned char*)(&d);

if(! ( c[0] ==  89 &&
       c[1] == 125 &&
       c[2] == 225 &&
       c[3] == 144 &&
       c[4] == 112 &&
       c[5] ==  88 &&
       c[6] == 234 &&
       c[7] ==  33    ) ) {
    /******************
    * not IEEE format *
    ******************/
    throw HardwareException("double is not IEEE format");

}


} // end of checkIEEE method


/****************************************************************************
* Check an istream or ostream for errors and throw the appropriate exception
* if there was one.
****************************************************************************/
void Interpreter::checkStream(ios* stream, const string& context) 
                  throw(IOException) {

/*
    cout << "checkStream good=" << stream->good() 
         << " fail="<<stream->fail()
         << " bad="<<stream->bad()
         << " eof=" << stream->eof() << "\n";
*/

    if(stream->good() ) return;
    if(stream->eof()  ) throw EOFException("EOF "+context);
    throw IOException(context);


} // end of private checkStream function


/****************************************************************************
* calculates 2 to some power and returns the result as a double.
* The result should be exact if the machine represents powers of two exactly.
* In other words it does a string of multiplications and does not use
* the exp() function.
****************************************************************************/
double Interpreter::powerOf2(int power) {

double place=2.0;
double value=1.0;

for(int n = abs(power); n!=0; n >>= 1 ) {

    if(n&1) value *= place;
    place *= place;

}

if(power>=0) return value;
else         return 1./value;

} // end of powerOf2 method

/****************************************************************************
* calculates the log base 2 of a number to the next lowest integer.
* it does this with a series of divisions and does not use the log()
* function, so the results are exact as long as multiplication and 
* division by powers of two are exact. This method is designed for use
* in encoding IEEE floating point numbers.
* This method assumes value >0.
* If a "remainder" argument is given the method will set it to value/2^log.
* remainder is guaranteed to be 1 <= remainder < 2.
****************************************************************************/
int Interpreter::logBase2(double value, double* remainder /*=NULL*/) {

int nmax=13;
double place[nmax];

if(value>=1.0) {
    /***************
    * positive log *
    ***************/
    place[0] = 2.;
    int i;
    for(i=0; i< nmax && place[i] <= value ; ++i)  {

        place[i+1] = place[i]*place[i];
    }

    --i;

    int lg=0;
    for( ; i>=0; --i) {

//printf("i=%d place=%g value=%g\n",i,place[i], value);
        lg <<= 1;
        if(place[i]<=value) {
        lg |= 1;
            value /= place[i];
        }

    }

//cout << "value ="<<value<<"\n";
    if(remainder!=NULL) *remainder=value;
    return lg;

} else {
    /***************
    * negative log *
    ***************/
    place[0] = 0.5;
    int i;
    for(i=0; i< nmax && place[i] >= value ; ++i)  {

        place[i+1] = place[i]*place[i];
    }

    --i;

    int lg=0;
    for( ; i>=0; --i) {

//printf("    i=%d place=%g value=%g\n",i,place[i], value);
        lg <<= 1;
        if(place[i]>=value) {
        lg |= 1;
            value /= place[i];
        }

    }

    if(value != 1.0) {
        ++lg;
        value *=2.;
    }

//cout << "value ="<<value<<"\n";

    if(remainder!=NULL) *remainder=value;
    return -lg;


}

   

} // end of logBase2 method




