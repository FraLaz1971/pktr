#include "LDPStubPacketBinner.h"

#include <sstream>
#include <iomanip>
#include <set>
#include <fstream>

#include "PacketReader.h"

/*********************************************************************
* constructor
*********************************************************************/
LDPStubPacketBinner::LDPStubPacketBinner() {

    _base = "swift_ldp_";
    _ext = ".ccsds";

} // end of constructor

/*********************************************************************
*
*********************************************************************/
LDPStubRepository* LDPStubPacketBinner::getRepository(int apid) {

    if(repositories.count(apid)) return repositories[apid];

    LDPStubRepository* repository = new LDPStubRepository();
    repositories[apid] = repository;
    return repository;

} // end of getRepository method

/*********************************************************************
*
*********************************************************************/
File* LDPStubPacketBinner::getFile(CCSDSPacket* generic_p) {

    /*************************************************
    * cast the packet as an LDP packet. If it isn't
    * then pass the packet through the pipe
    *************************************************/
    LDPPacket* packet = dynamic_cast<LDPPacket*>(generic_p);
    if(packet == NULL) return NULL;
    
//if(packet->apid() != 567 ) return NULL;

    /*********************************************
    * use the repository to determine the LDP.
    * if we can't determine the LDP, then pass the
    * packet through
    *********************************************/
    LDPStub* ldp = getRepository(packet->apid())->addPacket(packet);
    if(ldp==NULL) return NULL;

    /*************************************
    * now we can construct the file name *
    *************************************/
    ostringstream tmp;

    tmp << _base
        << hex << setfill('0') << setw(4) << ldp->getProduct()
        << _separator

        << setw(4) << ldp->getIndex()
        << _separator

        << dec << packet->apid()
        << _ext;
    #ifdef FAKING_STRINGSTREAM
    tmp << ends;
    #endif

    string file = tmp.str();

    #ifdef FAKING_STRINGSTREAM
    delete tmp.str();
    #endif
 //   cout << "LDPStubPacketBinner::getFile filename="<<file<<endl;



    return new File(_dir, file );

} // end of getFile method

/*****************************************************************************
*
*****************************************************************************/
void LDPStubPacketBinner::initRepositories() {

cout << "LDPStubPacketBinner::initRepositories start"<<endl;

    set<string> files;
    
    LDPPacket* packet = new LDPPacket();

     File::list(files, _dir + "/"+_base+"*"+_ext);
     for(set<string>::iterator it = files.begin(); it != files.end(); ++it) {
     
         string file = *it;
     
       //  cout << "init file: "<< file << endl;
         
         /*************************************
         * parse the index name from the file
         *************************************/
         int offset = file.find_last_of('/');
         int index;
         sscanf(file.substr(offset+16, 4).c_str(), "%x", &index);

         /**************************************
         * read the first packet from the file
         **************************************/
         ifstream in(file.c_str());
         PacketReader r(&in, packet);
         
         CCSDSPacket* generic_p;
         r >> generic_p;
         
         LDPPacket* p = dynamic_cast<LDPPacket*>(generic_p);


       //  cout << "apid="<<p->apid()<<" index="<<index<<endl;
         
         /************************************************
         * add the first packet to the proper repository *
         ************************************************/
         LDPStubRepository* repository = getRepository(p->apid());
         repository->initWith(p, index);



     } // end of loop over files

     delete packet;



} // end of initRepositories method





