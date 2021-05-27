#ifndef APIDENTIFIER_H
#define APIDENTIFIER_H

#include <map>
#include <string>

using namespace std;

/*****************************************************************************
* This class is basicly a map between APIDs and packets types for the 
* Swift mission. The mapping information is read from a configuration file
* by the constructor. 
* 
* The configuration file has one line per APID, with
* blank lines and lines beginning with "#" ignored.
* A line should contain the APID (in decimal), then whitespace, followed
* by the packet type. Any text following the packet type is ignored and 
* may be used to decribe the packet contents.
*
* Valid packet types are:
* - head2 = SwiftPacket,
* - head3 = SwiftHead3Packet,
* - uvot = SwiftPacket which can be CCSDS merged to form SwiftHead3Packets,
* - ldp  = LDPPacket,
* - checksum = ChecksumLDPPacket.
*****************************************************************************/
class APIDentifier {

public:
    /*************
    * class data *
    *************/
    static const int UNKNOWN =0;
    static const int HEAD2   =1;
    static const int HEAD3   =2;
    static const int UVOT    =3;
    static const int LDP     =4;
    static const int CHECKSUM=5;
    static const int ACS     =6;
    static const int TDRSS   =7;
    static const int TRASH   =8;

private:
    /*******
    * data *
    *******/
    map<int,int> types;

public:
    /**************
    * constructor *
    **************/
    APIDentifier(const string& file);

    /**************
    * destructor *
    **************/
    virtual ~APIDentifier() {}

    
    /**********
    * methods *
    **********/
    virtual int type(int apid) { return types[apid]; }
    //virtual CCSDSPacket* packet(apid)

private:
    /***********************
    * internal use methods *
    ***********************/
    virtual int parseTypeString(string word);
    

}; // end of APIdentifier class

#endif //  APIDENTIFIER_H
