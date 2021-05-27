#include "PacketBinner.h"

#include <sstream>
#include <fstream>
#include <iomanip>

/****************************************************************************
* constructor
****************************************************************************/
PacketBinner::PacketBinner() {


_separator = "_";
max_files=10;

} // end of constructor



/****************************************************************************
* destructor
****************************************************************************/
PacketBinner::~PacketBinner() {

   // cout << "destructing PacketBinner "<<this<<"\n";

    while(getFileCount() > 0 ) {

        closeFile();
    }


} // end of constructor

/****************************************************************************
* returns the number of open files
****************************************************************************/
int PacketBinner::getFileCount() { return filenames.size(); }

/****************************************************************************
* Close the oldest file
****************************************************************************/
void PacketBinner::closeFile() {

//cout <<"PacketBinner::closeFile start" <<endl;

    /******************
    * just to be sure *
    ******************/
    if(getFileCount() <=0) return;

    /**********************************
    * get the name of the oldest file *
    **********************************/
    string old_file = filenames.front();
    filenames.pop_front();

    /**************************
    * get the writing classes *
    **************************/
    PacketWriter* pw = writers[old_file];
    Writer* w = pw->getWriter();
    ostream* stream = w->getStream();

   // cout << "    closing "<<old_file<<" count= "<<getFileCount()<<endl;

    delete pw;
    delete w;

    /************************
    * close the file itself *
    ************************/
    stream->flush();

    if(! stream->good() ) {
        cerr << "An error occured writing to file "<<old_file
             << " eof="<<stream->eof()
             << " bad="<<stream->bad()
             << " fail="<<stream->fail()
             << endl;
    }

    ofstream* file= dynamic_cast<ofstream*>(stream);
    if(file) file->close();
    delete stream;
    
    /***************************************
    * remove the file from the writers map *
    ***************************************/
    writers.erase(writers.find(old_file));

} // end of closeFile method

/****************************************************************************
* This overrides the genric pipe method to write the packet into the 
* appropriate bin file.
****************************************************************************/
void PacketBinner::pushIn(CCSDSPacket* p) {

//cout << "PacketBinner::pushIn start" <<endl;

PacketWriter* w = getWriter( getFile(p) );

/*********************************************************************
* if we couldn't get a writer, then push the packet through the pipe 
* otherwise write the packet to the appropriate writer
*********************************************************************/
if(w == NULL ) {
    if(sink != NULL) *sink << p;
} else {
    *w << p;
}

} // end of push in method




/****************************************************************************
* Construct a File object for a given apid, time, and id.
* The time is included in the file name, to insure that we have a unique
* identifier even if the id can repeat itself. This way we can distinguish
* e.g. id=6 on 2001-08-29 from id=6 on 2001-09-01.
* Note that the units of time are arbitrary. Subclasses should choose
* a time unit which is longer than the time spanned by the packets in a
* given file, but shorter than the period over which the id can repeat itself.
* See the getFileNearTime() method for more information.
*****************************************************************************/
File* PacketBinner::getFile(int apid, int time, unsigned int id) {

ostringstream tmp;

/****************************************************************
* check for invalidly large times and set them to zero
* This is to ensure that the time field is never more than 4
* characters. The proc script needs this to glob telemetry
* file names properly
***************************************************************/
if(time > 0xffff || time < 0) time=0;


tmp << hex << setfill('0') << setw(8) << id
    << _separator
    << setw(4) << time;
#ifdef FAKING_STRINGSTREAM
tmp << ends;
#endif

ostringstream tmp2;
tmp2 << apid;
#ifdef FAKING_STRINGSTREAM
tmp2 << ends;
#endif

string dir = _dir + "/" + tmp.str();
string file = _base + tmp.str() + _separator + tmp2.str() + _ext;

#ifdef FAKING_STRINGSTREAM
delete tmp.str();
delete tmp2.str();
#endif

/*
if(apid==400) cout << "dir="<<dir
                   <<" file="<<file
                   <<" time="<<time
                   <<endl;
*/


//cout << "filename=|"<<tmp.str()<<"|\n";

//return new File(dir.c_str(), file.c_str() );
return new File(dir, file );


} // end of getFile method


/****************************************************************************
* returns a File object for a file with a time indicator within one
* of the given time, preferably one which already exists.
* This takes care of the problem that a time indicator may increment
* by one in the middle of a bin file (i.e. the bin spans "midnight").
****************************************************************************/
File* PacketBinner::getFileNearTime(int apid, int time, unsigned int id) {

File* before = getFile(apid, time-1, id);
File* during = getFile(apid, time  , id);
File* after  = getFile(apid, time+1, id);

// cout << "before="<<before->name() <<"\n";
// cout << "during="<<during->name() <<"\n";
// cout << "after="<<after->name() <<"\n";

if(during->directoryExists() ) {
    delete before;
    delete after;
    return during;
}

if(before->directoryExists() ) {
    delete during;
    delete after;
    return before;
}

if(after ->directoryExists() ) {
    delete before;
    delete during;
    return after;
}


// cout << "nothing exists\n";

/*****************************************
* no directory exists yet, so create one *
*****************************************/
File* dir = during->directory();
dir->makeDirectory();
delete dir;

delete before;
delete after;
return during;

} // end of getFileNearTime method



/****************************************************************************
* return the PacketWriter for a particular File object, creating one if
* it does not already exist. Note this method deletes the File object when it
* is done with it.
****************************************************************************/
PacketWriter* PacketBinner::getWriter(File* f) {

//cout << "PacketBinner::getWriter: f="<<f<<endl;

if(f==NULL) return NULL;

/***************************************
* first try getting an existing writer *
***************************************/
PacketWriter* w = writers[f->name()];
if(w != NULL ) {
    delete f;
    return w;
}

//cout << this << " has no writer for "<< f->name() <<endl;

/**************************************************
* if we get here we will need to open a new file
* so first check if we have too many files open
*************************************************/
while(max_files >0 && getFileCount() >= max_files) closeFile();

//cout << "    opening file "<<f->name() <<endl;
/**********************
* create a new writer *
**********************/
w = new PacketWriter(f->writer());
writers[f->name()] = w;
filenames.push_back(f->name());
delete f;
return w;

} // end of getWriter method


