#ifndef BAT_SIM_H
#define BAT_SIM_H

#include "ChecksumLDPPacket.h"
#include "PacketWriter.h"
#include "StandardLDPHead.h"
#include "LDPEncodeBuffer.h"

class BATSim {

private:
    /*******
    * data *
    *******/
    int size;
    LDPPacket* packet;
    PacketWriter* writer;
    double packet_time_interval;
    

public:
    /**************
    * constructor *
    **************/
    BATSim();

    /**********
    * methods *
    **********/
    void observe(double ra, double dec, int target, int obs, 
                 double end_time);

}; // end of BATSim class

#endif // BAT_SIM_H
