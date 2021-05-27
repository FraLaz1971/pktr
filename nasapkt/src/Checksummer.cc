#include "Checksummer.h"

/*********************************************************************
* constructor
*********************************************************************/
Checksummer::Checksummer() {
} // end of constructor

/*********************************************************************
*
*********************************************************************/


/*****************************************************************************
* write a byte.
* Returns EOF on error and 0 otherwise.
* This method overrides the generic strerambuf method to add the given byte 
* the current checksum.
*****************************************************************************/
int Checksummer::overflow(int c /*= EOF*/) {

if(c != EOF) addToSum((unsigned char)c);

return 0;

} // end of overflow method
