#ifndef PACKET_BINNER_H
#define PACKET_BINNER_H

#include <string>
#include <map>
#include <deque>

#include "PacketPipe.h"
#include "PacketWriter.h"
#include "File.h"

/****************************************************************************
* This is an abstract base class for subclasses which bin a
* stream of packets into different files depending on some criterion.
*
* This class maintains a list of files and PacketWriters for writing to those 
* files. It also provides functionality for appending a time value to an
* arbitrary integer ID, in order to make that ID unique for the life of the
* mission. Note this is non-trivial, since an ID may remain valid past
* "midnight" when the time value increments.
*
* Sub-classes need only override the getFile() method to specify the bin file
* to which a packet should be directed. The getFile() and getFileNearTime()
* methods will probably be useful for this.
* 
* Packets which cannot be binned come out the other side of the pipe. 
* Therefore it is probably a good idea to connect something downstream
* to catch these packets.
*
* It really only makes sense to push packets into a PacketBinner. If you try
* to pull a packet out, it will simply pull a packet from the upstream
* connection like an empty pipe, bypassing all binning. This is probably
* not what you want.
****************************************************************************/
class PacketBinner : public PacketPipe {

private:
    /*******
    * data *
    *******/
    map<string, PacketWriter*> writers;
    deque<string> filenames;
    int max_files;

protected:
    string _dir;
    string _base;
    string _ext;
    string _separator;

    
    

public:
    /**************
    * constructor *
    **************/
    PacketBinner();

    /*************
    * destructor *
    *************/
    virtual ~PacketBinner();


    /************
    * accessors *
    ************/
    virtual void dir(const string& dir) { _dir = dir; }
    virtual void base(const string& base) { _base = base; }
    virtual void ext(const string& ext) { _ext = ext; }
    virtual void separator(const string& sep) { _separator = sep; }


    /**********
    * methods *
    **********/
    virtual void pushIn(CCSDSPacket* packet);

//protected:
public:
    /***************************
    * internal utility methods *
    ***************************/
    virtual File* getFile(int apid, int time, unsigned int id);
    virtual File* getFileNearTime(int apid, int time, unsigned int id);
    virtual PacketWriter* getWriter(File* f);

    /*****************************************************************
    * completely virtual methods which must be defined in subclasses *
    *****************************************************************/
    virtual File* getFile(CCSDSPacket* p) = 0;
    
private:
    /**************************************
    * open file repository methods *
    *******************************/
    virtual void closeFile();
    virtual int getFileCount();

}; // end of PacketBinner class

#endif //  PACKET_BINNER_H
