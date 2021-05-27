#include "PacketCounter.h"

/*********************************************************************
* constructor
*********************************************************************/
PacketCounter::PacketCounter() {

reset();

} // end of constructor


/*********************************************************************
* return the current packet count in a given APID
*********************************************************************/
int PacketCounter::getCount(int apid) { 

    if(count.count(apid) >0) return count[apid];
    else                     return 0;

} // end of getCount by APID method

/*********************************************************************
* return the current packet count summed over APIDS
*********************************************************************/
int PacketCounter::getCount() { return total; }





/*********************************************************************
* reset the packet count to zero
*********************************************************************/
void PacketCounter::reset() {

total=0;
count.erase(count.begin(), count.end());

} // end of reset method


/*********************************************************************
* count a single packet moving through the pipe
*********************************************************************/
CCSDSPacket* PacketCounter::operateOnPacket(CCSDSPacket* p) {

if(p==NULL) return p;

int apid = p->apid();

//cout << apid<<"\n";

/***************************************************************
* make sure the count is initialized, since I'm not sure what
* the map does by default
***************************************************************/
if(count.count(apid) == 0 ) count[apid] = 0;

/**********************
* increment the count *
**********************/
++count[apid];
++total;

return p;


} // end of operateOnPacket method


/*********************************************************************
* print a report of all APIDS and counts to a given ostream
*********************************************************************/
void PacketCounter::report(ostream* out) {

for(map<int,int>::iterator it = count.begin();
    it != count.end(); ++it ) {

    cout << "APID "<<it->first 
         << " (0x"<<hex<<it->first<<dec<<")"
         <<" "<< it->second << " packets\n";

}

cout << total<<" packets total\n";

} // end of report method
