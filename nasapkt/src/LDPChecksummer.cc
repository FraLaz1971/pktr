#include "LDPChecksummer.h"

/*********************************************************************
* constructor
*********************************************************************/
LDPChecksummer::LDPChecksummer() {

sum=0;
modulus = 1<<16;

//cout << "make new checksummer --------------\n";

} // end of constructor

/*********************************************************************
* 
*********************************************************************/
void LDPChecksummer::addToSum(unsigned char c) { 

sum += c; 

//cout << "c="<<(int)c<<" sum="<<sum<<"\n";

}

/*********************************************************************
* 
*********************************************************************/
unsigned long LDPChecksummer::value() { 

//cout << "raw sum="<<sum<<" after modulo="<<sum%modulus<<"\n";

return (unsigned long)(sum%modulus); 

} // end of addToSum method
