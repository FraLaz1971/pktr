#include "LDPPacketRun.h"

/*********************************************************************
* constructor
*********************************************************************/
LDPPacketRun::LDPPacketRun(double time1, int seq1,
                           double time2, int seq2, int product) {
                           
    this->time1 = time1;
    this->time2 = time2;

    this->seq1 = seq1;
    this->seq2 = seq2;

    this->product = product;

} // end of constructor

/*********************************************************************
* construct a run with just one packet
*********************************************************************/
LDPPacketRun::LDPPacketRun(double time, int seq, int product) {

    this->time1 = time;
    this->time2 = time;

    this->seq1 = seq;
    this->seq2 = seq;

    this->product = product;
              
}

/**********************************************************************
* returns true if two runs have at least one packet in common
**********************************************************************/
bool LDPPacketRun::overlaps(LDPPacketRun* run) {

    /******************************************
    * now check if the times overlap
    *******************************************/
    if(time1 > run->time2 || time2 < run->time1) return false;
    
    /******************************************************************
    * if the end times coincide, then we need to check the sequence
    * numbers
    ******************************************************************/
    if(     time1 == run->time2) return      seq1 <=  run->seq2;
    else if(time2 == run->time1) return run->seq1 <=       seq2;

    /****************************************
    * if we get here then the times overlap *
    ****************************************/
    return true;

} // end of overlap method



/**********************************************************************
* returns true if the given packet is part of the same LDP as this run.
**********************************************************************/
bool belongsTo(LDPPacketRun* run) {

    /********************************************
    * first make sure the product numbers agree *
    ********************************************/
    if(product != run->product) return false;
    
    /******************************************************
    * if the runs overlap, then they belong to each other *
    ******************************************************/
    if(overlaps(run)) return true;

    /*****************************************************
    * we need to extrapolate to see if we match up with the 
    * other run
    ********************************************************/
    if(this < run) {
        /***************************************************
        * we are on the left, so extrapolate from the right *
        ***************************************************/


} // end of belongs to method

/*************************************************************7********
* Compare two runs based on the first packet in each. First compares
* the packet times and then compares the sequence numbers if the
* times are equal.
*********************************************************************/
bool operator<(const LDPPacketRun& left, const LDPPacketRun& right) {

    if(left.time1 < right.time1) return true;
    if(left.time1 == right.time1) return false;

    return left.sequence1 < right.sequence1;

} // end of less than operator

/**********************************************************************
* returns the difference in two sequence counters, taking rollover
* into account
**********************************************************************/
int sequenceDifference(int sequence1, int sequence2) {

    int diff = sequence1 - sequence2;
    if(diff < -SEQUENCE_HALF_PERIOD) diff += SEQUENCE_PERIOD;
    if(diff >  SEQUENCE_HALF_PERIOD) diff -= SEQUENCE_PERIOD;

    return diff;


} // end of sequenceDifference function
