#ifndef PACKET_TANK_H
#define PACKET_TANK_H

#include "CCSDSPacket.h"
#include "PacketPipe.h"
#include <deque>

/*****************************************************************************
* This is a tank for storing packets. If you push a packet in, it is held
* in an STL deque container and doesn't come out the other side. If you pull
* a packet out, it removes the first packet from the tank and delivers it.
* If there are no packets in the tank, it tries to pull one from upstream.
* Therefore a tank can act as a buffer if input and output are not
* synchronized for some reason.
*
* Probably the most important use of a tank is for sorting or uniquing packets.
* To do this you first fill the tank, then call the sort() and/or
* keepLastUnique() methods, then drain the tank.
*
* The container() method gives you direct access to the undelying deque if 
* you need to do something realy fancy to the collected packets.
*****************************************************************************/

int comes_beforel(CCSDSPacket* left, CCSDSPacket* right);


class PacketTank : public PacketPipe {

private:
    /*******
    * data *
    *******/
    deque<CCSDSPacket*>* tank;

    int unique_packets;

public:
    /**************
    * constructor *
    **************/
    PacketTank(); 

    /************
    * accessors *
    ************/
    virtual deque<CCSDSPacket*>* container() const {return tank; }

    virtual int hints();
    virtual void hints(int);

    /**********
    * methods *
    **********/
    virtual void sort();
    virtual void keepLastUnique(PacketPipe* drain = NULL);
    


    /***********************************************************
    * override PacketPipe methods to store packets in the tank *
    ***********************************************************/
    virtual void pushIn(CCSDSPacket* p);
    virtual CCSDSPacket* pullOut();
    virtual void pump();

}; // end of PacketTank class


#endif //  PACKET_TANK_H
