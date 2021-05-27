#ifndef LDPPACKET_LIST_H
#define LDPPACKET_LIST_H

class LDPPacketList {

private:
    /************************************************
    * embedded comparison class for sorting the set *
    ************************************************/
    class lessThan {
        public:
        bool operator()(const LDPPacketRun* left, const LDPPacketRun* right) {
            return *left < *right;
        }
    }; // end of embedded class for comparisons

private:
    /*******
    * data *
    *******/
    multiset<LDPPacketRun*, lessThan> s;
    multiset<LDPPacketRun*, lessThan>::iterator position;

public:
    /**************
    * constructor *
    **************/
    LDPPacketList();

    /*************
    * destructor *
    *************/
    virtual ~LDPPacketList() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    addPacket(double time, int sequence, int product);

}; // end of LDPPacketList class

#endif // LDPPACKET_LIST_H
