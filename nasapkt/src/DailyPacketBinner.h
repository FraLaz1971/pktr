#ifndef DAILY_PACKET_BINNER_H
#define DAILY_PACKET_BINNER_H

#include "PacketBinner.h"
#include "File.h"

class DailyPacketBinner : public PacketBinner {

private:
    /*******
    * data *
    *******/
    double day_length;
    double time0;
    int ndigits;


public:
    /**************
    * constructor *
    **************/
    DailyPacketBinner();
    DailyPacketBinner(double day_length, double time0, int ndigits);

    /*************
    * destructor *
    *************/
    virtual ~DailyPacketBinner() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual File* getFile(CCSDSPacket* generic_p);
    virtual File* getFileFromTime(int apid, double time);

}; // end of DailyPacketBinner class

#endif // DAILY_PACKET_BINNER_H
