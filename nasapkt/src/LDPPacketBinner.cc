#include "LDPPacketBinner.h"

#include <sstream>
#include <iomanip>


/****************************************************************************
* constructor
****************************************************************************/
LDPPacketBinner::LDPPacketBinner(int time_scale) {

this->time_scale = time_scale;
_base = "swift_ldp_";
_ext = ".ccsds";

} // end of constructor


/****************************************************************************
* return the file for a particular packet
****************************************************************************/
File* LDPPacketBinner::getFile(CCSDSPacket* generic_p) {


LDPPacket* p = dynamic_cast<LDPPacket*>(generic_p);
if(p==NULL) return NULL;

int time = (int)(p->time())/time_scale;


return getFileNearTime(p->apid(), time, p->product() );


} // end of getFile method


/****************************************************************************
* Construct a File object for a given apid, time, and id.
* The time is included in the file name, to insure that we have a unique
* identifier even if the id can repeat itself. This way we can distinguish
* e.g. id=6 on 2001-08-29 from id=6 on 2001-09-01.
* Note that the units of time are arbitrary. Subclasses should choose
* a time unit which is longer than the time spanned by the packets in a
* given file, but shorter than the period over which the id can repeat itself.
* See the getFileNearTime() method for more information.
*****************************************************************************/
File* LDPPacketBinner::getFile(int apid, int time, unsigned int id) {


/****************************************************************
* check for invalidly large times and set them to zero
* This is to ensure that the time field is never more than 4
* characters. The proc script needs this to glob telemetry
* file names properly
***************************************************************/
if(time > 0xffff || time < 0) time=0;


ostringstream tmp;

tmp << _base  
    << hex << setfill('0') << setw(4) << id
    << _separator
    << setw(4) << time
    << _separator
    << dec << apid     
    << _ext;
#ifdef FAKING_STRINGSTREAM
tmp << ends;
#endif

string file = tmp.str();

#ifdef FAKING_STRINGSTREAM
delete tmp.str();
#endif


//cout << "filename=|"<<tmp.str()<<"|\n";

return new File(_dir, file );


} // end of getFile method


/****************************************************************************
* returns a File object for a file with a time indicator within one
* of the given time, preferably one which already exists.
* This takes care of the problem that a time indicator may increment
* by one in the middle of a bin file (i.e. the bin spans "midnight").
****************************************************************************/
File* LDPPacketBinner::getFileNearTime(int apid, int time, unsigned int id) {

File* before = getFile(apid, time-1, id);
File* during = getFile(apid, time  , id);
File* after  = getFile(apid, time+1, id);

//cout << "before="<<before->name() <<"\n";
//cout << "during="<<during->name() <<"\n";
//cout << "after="<<after->name() <<"\n";

if(during->exists() ) {
    delete before;
    delete after;
    return during;
}

if(before->exists() ) {
    delete during;
    delete after;
    return before;
}

if(after ->exists() ) { 
    delete before;
    delete during;
    return after;
}

//cout << "nothing exists\n";

delete before;
delete after;
return during;

} // end of getFileNearTime method
