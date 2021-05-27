#include "APIDPacketBinner.h"

#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iomanip>

/****************************************************************************
* constructor
****************************************************************************/
APIDPacketBinner::APIDPacketBinner(const string& format) {

this->format = format.c_str();

filename = new char[format.length()+32];

_dir = ".";

} // end of constructor

/****************************************************************************
* destructor
****************************************************************************/
APIDPacketBinner::~APIDPacketBinner() {

    delete filename;
}

/****************************************************************************
* return the file for a particular packet
****************************************************************************/
File* APIDPacketBinner::getFile(CCSDSPacket* p) {

    if (p == NULL) return NULL;

    /*********************************************************
    * generate the file name using the APID and
    * the format specified in the constructor
    * Note that it should be safe to use sprintf this way
    * since the only variable thing is the APID and that 
    * has a limited size
    ********************************************************/
    sprintf(filename, format, p->apid());

    string dir = _dir + "/";
    return new File(dir, filename);

} // end of getFile method
