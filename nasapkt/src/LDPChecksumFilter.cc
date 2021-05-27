#include "LDPChecksumFilter.h"

#include "ChecksumLDPPacket.h"

/*********************************************************************
* constructor
*********************************************************************/
LDPChecksumFilter::LDPChecksumFilter() {
} // end of constructor


/*********************************************************************
*
*********************************************************************/
bool LDPChecksumFilter::accept(CCSDSPacket* p) {

    if(p==NULL) return true;

    /***************************************************
    * cast the packet as a checksummed LDP packet, and
    * pass through all other sorts of packets
    ***************************************************/
    ChecksumLDPPacket* ldp = dynamic_cast<ChecksumLDPPacket*>(p);
    if(ldp == NULL) return true;


    if(ldp->checksum() != ldp->calculateChecksum() ) {
        cerr << "bad checksum for"
             << " APID "      << ldp->apid()
             << " product "   << ldp->product()
             << " page "      << ldp->page()
             << " is "        << ldp->checksum()
             << " should be " << ldp->calculateChecksum()
           //  << " diff="      << (ldp->calculateChecksum() - ldp->checksum())
             << endl;
        return false;
    } else {
//             cerr << "product "<<ldp->product()
//              << " page "<<ldp->page()
//              << " is "<<ldp->checksum()
//              << endl;
        return true;
    }



} // end of accept method
