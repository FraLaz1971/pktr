#include "DailyPacketBinner.h"

#include <sstream>
#include <fstream>
#include <iomanip>

#include "SwiftHead3Packet.h"

/*********************************************************************
* constructor
*********************************************************************/
DailyPacketBinner::DailyPacketBinner(double day_length, double time0, int ndigits) {


    this->day_length = day_length;
    this->time0 = time0;
    this->ndigits = ndigits;

    _base = "swift_chunk_";
    _ext = ".ccsds";

} // end of constructor

/****************************************************************************
* return the file for a particular packet
****************************************************************************/
File* DailyPacketBinner::getFileFromTime(int apid, double time) {


int day = (int)((time-time0)/day_length);

//cout << "apid="<<apid<<" time="<<time<<" day="<<day<<endl;

ostringstream tmp;
tmp << hex << setfill('0')
    << setw(ndigits) << day;
#ifdef FAKING_STRINGSTREAM
tmp << ends;
#endif

ostringstream tmp2;
tmp2 << apid;
#ifdef FAKING_STRINGSTREAM
tmp2 << ends;
#endif

string dir = _dir + "/" + tmp.str();
string filename = _base + tmp.str() + _separator + tmp2.str() + _ext;

//cout << "filename="<<filename<<" dir="<<dir<<endl;

#ifdef FAKING_STRINGSTREAM
delete tmp.str();
delete tmp2.str();
#endif

/******************************************************************
* create the file object and make sure we have a directory for it *
******************************************************************/
File* file = new File(dir, filename );

if(!file->directoryExists()) {

    File* dir = file->directory();
    dir->makeDirectory();
    delete dir;
}

return file;

} // end of getFileFromTime method

/****************************************************************************
* return the file for a particular packet
****************************************************************************/
File* DailyPacketBinner::getFile(CCSDSPacket* generic_p) {


SwiftPacket* p = dynamic_cast<SwiftPacket*>(generic_p);

if(p) return getFileFromTime(p->apid(), p->bestTime() );
else  return NULL;



} // end of getFile from packet method

