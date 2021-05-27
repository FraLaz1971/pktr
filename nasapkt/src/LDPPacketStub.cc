#include "LDPPacketStub.h"

/*********************************************************************
* constructor
*********************************************************************/
LDPPacketStub::LDPPacketStub(LDPPacket* packet) {

    is_xrt = packet->apid() == 1344; //other XRT LDPs do not have this property

    time = packet->time();
    seq = packet->sequence();

    product = packet->product();
    page    = packet->page();
    
    /******************************
    * page count defaults to zero *
    ******************************/
    npages = 0;

    if(page==1) {
        /****************************************************
        * read the page count from the first two data bytes *
        ****************************************************/
        Reader* r = packet->dataReader();
        try { npages = r->readUnsignedInt(2); }
        catch(Interpreter::Exception& e) {}
        delete r;
    }

    /***************************************************
    * we don't know the LDP we belong to at this point *
    ***************************************************/
    ldp = NULL;

} // end of constructor

/*********************************************************************
*
*********************************************************************/
LDPStub* LDPPacketStub::getLDP() { return ldp; }

/*********************************************************************
* will set the page count in the LDP if this packet knows it
*********************************************************************/
void LDPPacketStub::setLDP(LDPStub* ldp) { 

    this->ldp = ldp; 
    
    if(page == 1 && npages != 0 ) ldp->setPageCount(npages);
    
}

/*********************************************************************
*
*********************************************************************/
int LDPPacketStub::getProduct() { return product; }



/*********************************************************************
* Returns the page count if known or zero if it isn't.
*********************************************************************/
int LDPPacketStub::getLDPPageCount() {

    if(ldp==NULL) return 0;
    else          return ldp->getPageCount();
    
}



/*********************************************************************
*
*********************************************************************/
bool LDPPacketStub::sameLDPAs(LDPPacketStub* stub) throw(LDPPacketStub::MaybeException) {

    /******************************************************
    * different product number always means different LDP *
    ******************************************************/
    if(product != stub->product) return false;
    
    /******************************************************
    * if the packets are the same, then return true
    ******************************************************/
    if(*this == *stub) return true;

    /****************************************************
    * make sure the "is_xrt" flags are the same.
    * packets from different APIDs can't be from the same LDP
    **********************************************************/
    if(is_xrt != stub->is_xrt ) return false;
    
#ifdef DEBUG
if(product==1) cout << "    past xrt check\n";
#endif


    /*****************************************************
    * check for a reset in the sequence counter
    * This would indicate a reboot happened between the
    * two packets, which would mean a break in LDPs
    ****************************************************/
    if(time>stub->time && seq < stub->seq) return false;
    if(time<stub->time && seq > stub->seq) return false;
    
#ifdef DEBUG
if(product == 1) cout << "    past seq reset check"<<endl;
#endif
    
    /**************************************************************
    * check if the two packets are so far apart in time that they 
    * couldn't be part of the same LDP
    **************************************************************/
    if(fabs(time-stub->time) > MAX_LDP_DURATION ) return false;
#ifdef DEBU
if(product == 1) cout << "    past max LDP duration check"<<endl;
#endif
    /********************************************************
    * look for big time gaps between packets. The packets
    * within an LDP are typically no more than
    * a fraction of a second apart, though sometimes
    * they are as much as about a second apart.
    *******************************************************/
#ifdef DBEUG
if(product==1) cout << "    delta time="<<(time-stub->time)
                     << " delta seq=" <<(seq - stub->seq)
                     << " time per packet="<<(time-stub->time)/(seq - stub->seq)
                     << " limit="<<MAX_TIME_PER_PACKET
                     << endl;
#endif
    if((time-stub->time)/(seq - stub->seq) > MAX_TIME_PER_PACKET) return false;

#ifdef DEBUG
if(product == 1) cout << "    past time per packet test"<<endl;
#endif

    /****************************************************************
    * make sure the difference in packets is not larger than the
    * maximum number of pages in an LDP. This check doesn't
    * do anything unless the maximum LDP size can be restricted
    * to being smaller than half the period of the sequence counter
    ****************************************************************/
    if(abs(seq - stub->seq) > MAX_LDP_PAGES) return false;
    
    /***********************************************
    * now we can check the page ordering
    ***********************************************/
    if(page != 1 && stub->page != 1) {
        /*********************************************
        * neither is a page1, so the check is simple *
        *********************************************/
#ifdef DBEUG
if(product == 1) cout << "    neither is a page1"<<endl;
#endif
        return (page - stub->page) == (seq - stub->seq);
    } else {
        /***************************************************
        * things get tricky when a page1 is involved, since 
        * we don't always know if the page1 is low or high
        ***************************************************/
        LDPPacketStub* earlier;
        LDPPacketStub* later;
        if(*this < *stub) {
            earlier = this;
            later   = stub;
        } else {
            earlier = stub;
            later   = this;
        }
#ifdef DEBUG
if(product==1) cout << "    earlier page="<<earlier->page<<" later page="<<later->page<<endl;
#endif
        if(earlier->page == 1 && later->page == 1 ) {
            /**************************************************
            * both are page1s. First check if the later one
            * has a zero page count, since the final page1
            * always has a non-zero page count. For XRT the 
            * first page1 always has a zero page count
            **************************************************/

            if(later->npages == 0 ) return false;
            if(earlier->is_xrt && earlier->npages != 0 ) return false;
            
            /********************************
            * the page counts have to agree *
            ********************************/
            if(earlier->npages != 0 && earlier->npages != later->npages) return false;
            
            /*************************************************
            * at this point we're sure of the page count, so 
            * we can check if we the distance between the
            * packets is correct. Note that this can give a false
            * positive if the first page count is unknown, but
            * this is not an XRT packet.
            **************************************************/
            if(later->seq - earlier->seq != later->npages ) return false;
            if(later->is_xrt || earlier->npages != 0) return true;
            
            /*****************************************************
            * NOTE at this point I'm stumped
            * but we might not ever get here if the BAT always
            * fills in the page count in the initial page1.
            * David Palmer sez this is the case 2004-04-27
            *****************************************************/
            throw MaybeException();


        } else if(earlier->page == 1) {
#ifdef DEBUG
if(product==1) cout << "    page1 to the left"<<endl;
if(product==1) cout << " earlier npages="<<earlier->npages<<" is_xrt="<<earlier->is_xrt<<endl;
#endif
            /******************************************************
            * The earlier packet is a page1, so if both packets
            * are to be part of the same LDP, then it must be 
            * an initial page1.
            * NOTE: This fails for busted XRT telemetry, since
            * the initial page1 are soemtimes missing.
            * However, we can save the day by first checking whether
            * the page count was zero, since the initial page1
            * always has a zero page count for XRT.
            *****************************************************/
            if(earlier->is_xrt && earlier->npages != 0 ) return false;
#ifdef DEBUG
if(product==1) cout << "    delta page="
<<(later->page - earlier->page)
<< " delta seq="<<(later->seq - earlier->seq)
<<endl;
#endif
            return (later->page - earlier->page) == (later->seq - earlier->seq);
        } else {
            /******************************************************
            * the later packet is a page1, so for them to be part
            * of the same LDP, the later packet must be a final 
            * page1. First we check the page count. A final page1
            * must always have a non-zero page count.
            ******************************************************/
            if(later->npages == 0 ) return false;
            
            /************************************************
            * now check if we are in the right place for a 
            * packet which comes just after the last page.
            * Note this check could give a false positive if
            * the number of pages in the following LDP
            * is one greater than the number of pages in the
            * previous LDP.
            ***********************************************/
            if(later->npages + 1 - earlier->page !=
               later->seq - earlier->seq            ) return false;

            /********************************************************
            * for the XRT, we know definitively whether we are an
            * initial or final page1 by looking at the page count.
            * so in that case the above test has no false positives
            ********************************************************/
            if(later->is_xrt ) return true;
               
            /***********************************************
            * if the earlier packet is right next to us then 
            * we know we are part of it
            *************************************************/
            if(later->seq - earlier->seq == 1 ) return true;
            
            /*****************************************************
            * we might have some page count info if the earlier packet
            * has already been associated with a page1.
            ************************************************************/
            int ldp_pages = earlier->getLDPPageCount();
            if(ldp_pages != 0) {
                /*******************************************
                * OK< we do know trhe page count of the LDP 
                * corresponding to the earlier packet.
                * first make sure the pages counts match
                ********************************************/
                if(ldp_pages != later->npages) return false;
                
                /*********************************************
                * now check to see if we are in the expected
                * place based on the LDP page count
                *********************************************/
                return ldp_pages + 1 - earlier->page !=
                       later->seq - earlier->seq;
            }

            /****************************************************************
            * NOTE: at this point I'm stumped. The basic problem is we
            * don't know the page count for the LDP on the left.
            * we have the page count in the page1, but we can't be sure 
            * that's not the page copunt for a different LDP.
            ****************************************************************/
            throw MaybeException();


        } // end if the later paget is a page1

    } // end if there is a page1


    




} // end of sameLDPAs method


/*************************************************************7********
* Compare two runs based on the first packet in each. First compares
* the packet times and then compares the sequence numbers if the
* times are equal.
*********************************************************************/
bool operator<(const LDPPacketStub& left, const LDPPacketStub& right) {

    if(left.time < right.time) return true;
    if(left.time > right.time) return false;

    return left.seq < right.seq;

} // end of less than operator

/*************************************************************7********
* Compare two runs based on the first packet in each. First compares
* the packet times and then compares the sequence numbers if the
* times are equal.
*********************************************************************/
bool operator==(const LDPPacketStub& left, const LDPPacketStub& right) {

    return left.time == right.time && left.seq == right.seq;

} // end of equals operator
