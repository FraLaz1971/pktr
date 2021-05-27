#ifndef LDP_CHECKER_H
#define LDP_CHECKER_H

#include "PacketPipe.h"
#include "LDPPacket.h"
#include "StandardLDPHead.h"


/*****************************************************************************
* This is a kind of PacketPipe which checks the packets passing through it
* to be sure they conform to correct Swift LDP structure.
* It checks to be sure there is a redundant header, but does
* not transmit it downstream.
* If there is an error, it stops transmitting packets and the 
* error() method will return a pointer to a string describing the error.
* If there was no error, error() will return NULL.
* Missing packets are considered to be an error.
*
*****************************************************************************/
class LDPChecker : public PacketPipe {

public:
    /*************
    * class data *
    *************/
    static const int first_page=1;

private:
    /*******
    * data *
    *******/
    int on_first_page;
    int in_redundant_header;

    int npages;

    int last_page;
    int last_apid;
    int last_product;
    int last_sequence;
    double last_time;

    StandardLDPHead* head;
    StandardLDPHead* redundant_head;
    

   // char* _error;
    string _error;
    bool found_error;

public:
    /**************
    * constructor *
    **************/
    LDPChecker() {
        on_first_page=1;
        in_redundant_header=0;

        npages=0;
        head=NULL;
        redundant_head=NULL;
        last_page=first_page-1;

        found_error = false;

    } // end of constructor

    /*************
    * destructor *
    *************/
    virtual ~LDPChecker() {
      //  free(_error);
    }

    /***********
    * accessor *
    ***********/
    virtual void error(const string&  e) { _error=e; found_error=true; }
    virtual bool foundError() { return found_error; }
    virtual const string& error() { return _error; }


protected: 
    
    CCSDSPacket* operateOnPacket(CCSDSPacket* generic_p); 

};


#endif //  LDP_CHECKER_H
