#ifndef LDPPACKET_RUN_H
#define LDPPACKET_RUN_H

#include "Sequence.h"

class LDPPacketRun {

private:
    /*******
    * data *
    *******/
    double time1;
    Sequence seq1;

    double time2;
    Sequence seq2;
    
    int product;


public:
    /**************
    * constructor *
    **************/
    LDPPacketRun(double time1, int seq1,
                 double time2, int seq2, int product);
                 
    LDPPacketRun::LDPPacketRun(double time, int seq, int product);

    /*************
    * destructor *
    *************/
    virtual ~LDPPacketRun() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    bool overlaps(LDPPacketRun* run);
    bool belongsTo(LDPPacketRun* run);

    /************
    * operators *
    ************/
    friend bool operator<(const LDPPacketRun& left, const LDPPacketRun& right);
    
    /*******************
    * statuc utilities *
    *******************/
    int sequenceDifference(int sequence1, int sequence2);

}; // end of LDPPacketRun class

#endif // LDPPACKET_RUN_H
