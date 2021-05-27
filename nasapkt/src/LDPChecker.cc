#include "LDPChecker.h"

#include "DataBuf.h"
#include <sstream>

/******************************************************************************
* check a number of things in this packet to make sure it's in the right format
*******************************************************************************/
CCSDSPacket* LDPChecker::operateOnPacket(CCSDSPacket* generic_p) {

/******************************************************
* a NULL pointer means "end of stream", so pass it on *
******************************************************/
if(generic_p == NULL) return NULL;

/**********************************************
* stop transmitting if we have found an error *
**********************************************/
if(found_error) return NULL;

/**********************************
* make sure we have an LDP packet *
**********************************/
LDPPacket* p = dynamic_cast<LDPPacket*>(generic_p);

if(p==NULL) {
    error("Not an LDP packet");
    return NULL;
}


/***********************************************
* checks we can perform on individual packets: *
***********************************************/
if(p->type() != 0 ) {
    ostringstream tmp;
    tmp << "CCSDS primary header type flag set to " << p->type();
#   ifdef FAKING_STRINGSTREAM
    tmp<<ends;
#   endif
    error(tmp.str());
    return NULL;
}

if(!(p->isFirst()) || !(p->isLast()) ) {
    ostringstream tmp;
    tmp << "CCSDS primary header grouping flags set to "
        << p->isFirst() << p->isLast();
#   ifdef FAKING_STRINGSTREAM
    tmp<<ends;
#   endif

    error(tmp.str());
    return NULL;
}

if(on_first_page) { 
    /*****************************************************
    * this is the first page, so reset the first page
    * flag for the next page
    *****************************************************/
    on_first_page=0;

    /******************************************************
    * read the standard header if the first packet is big 
    * enough to hold it
    ******************************************************/
    if(p->size() >= StandardLDPHead::size ) {
        head = new StandardLDPHead();
        istream s( new DataBuf((char*)(p->data()), p->size()));
        Reader r(&s);
        head->read(&r);

        npages=head->npages();
        cout << "npages="<<npages<<"\n";
    }

    /*************************************************
    * check that the first page has the right number *
    *************************************************/
    if(p->page() != first_page) {
        ostringstream tmp;
        tmp << "First page is numbered "<< p->page();
#       ifdef FAKING_STRINGSTREAM
	tmp << ends;
#       endif
        error(tmp.str());
        return NULL;
    }
    
} else {
    /***********************************************
    * this is not the first packet, 
    * so check it against the previous packet 
    ***********************************************/

    
    /************************************************************
    * check if this is the first packet of the redundant header *
    ************************************************************/
    if(!in_redundant_header &&
       p->page()==first_page && 
       (last_page - first_page +1 == npages || npages==0) ) {
        /***************************************************
        * this is the first packet of the redundant header *
        ***************************************************/
        in_redundant_header=1;
        cout << "in redundant header\n";

        /*******************************************************
        * read the standard header from the redundant packet
        * if it is big enough to contain it
        *******************************************************/
        if(p->size() >= StandardLDPHead::size ) {
            redundant_head = new StandardLDPHead();
            istream s( new DataBuf((char*)p->data(), p->size()));
            Reader r(&s);
            redundant_head->read(&r);

            /***************************************************************
            * check that the page count is correct in the redundant header *
            ***************************************************************/
            if((int)redundant_head->npages() != last_page - first_page +1 ) {
                ostringstream tmp;
                tmp << "Page count in redundant header "
                    << redundant_head->npages()
                    << " is different from the total number of pages "
                    << (last_page - first_page +1);
#               ifdef FAKING_STRINGSTREAM
                tmp<< ends;
#               endif
                error(tmp.str());
            } // end if page count is wrong


            /**************************************************
            * check that the tertiary header part is the same *
            **************************************************/
            if(head && *(head->head3()) != *(redundant_head->head3()) ) {
                    error("Redundant header has different information");

            }
        }

        /*****************************************
        * read and check the rest of the packets *
        *****************************************/
        CCSDSPacket* head_p;
        while((head_p = source->pullOut()) && operateOnPacket(head_p) );  

        /************************************
        * ...but don't transmit any of them *
        ************************************/
        return NULL;   
       
    } else {
        /********************************************
        * check if pages are incrementing correctly *
        ********************************************/
        if(p->page() != last_page+1 ) {
 
            ostringstream tmp;
            tmp << "Page "
                << p->page() << " follows " 
                << last_page << "\n";
#           ifdef FAKING_STRINGSTREAM
            tmp<<ends;
#           endif
            error(tmp.str() );
            return NULL;

        }
    } // end of page number check

    /*******************************
    * APIDs should all be the same *
    *******************************/
    if((int)p->apid() != last_apid ) {
        ostringstream tmp;
        tmp << "Inconsistent APIDs "
            << last_apid << " and " 
            <<   p->apid() << "\n";
#       ifdef FAKING_STRINGSTREAM
        tmp<<ends;
#       endif

        error(tmp.str() );
        return NULL;

     }

    /*****************************************
    * product numbers should all be the same *
    *****************************************/
    if(p->product() != last_product ) {
        ostringstream tmp;
        tmp << "Inconsistent Product IDs "
            << last_product << " and " 
            <<   p->product() << "\n";
#       ifdef FAKING_STRINGSTREAM
        tmp<<ends;
#       endif

        error(tmp.str() );
        return NULL;
    }


    /************************************
    * sequence counter should increment *
    ************************************/
    if((int)p->sequence() != last_sequence+1 &&
       !(p->sequence()==0 &&  last_sequence == (1<<14)-1 ) ) {
        ostringstream tmp;
        tmp << "Sequence "
            <<   p->sequence() << " follows " 
             << last_sequence << "\n";
#       ifdef FAKING_STRINGSTREAM
        tmp<<ends;
#       endif

        error(tmp.str() );
        return NULL;
    }

    /***************************
    * time should not decrease *
    ***************************/
    if(p->time() < last_time ) {
        ostringstream tmp;
        tmp << "Secondary header time "
            <<   p->time() << " follows " 
            << last_time << "\n";
#       ifdef FAKING_STRINGSTREAM
        tmp<<ends;
#       endif

        error(tmp.str() );
        return NULL;
    }


    

} // end if checking a pair of packets


/****************************************************************
* save information from the packet to compare with the next one *
****************************************************************/
last_page     = p->page();
last_apid     = p->apid();
last_product  = p->product();
last_sequence = p->sequence();
last_time     = p->time();


/***********************************************************
* send the packet on unless we are in the redundant header *
***********************************************************/
if(in_redundant_header) return NULL;
else                    return p;


} // end of operateOnPacket method
