#ifndef FILE_H
#define FILE_H

#include <string>
#include <set>

#include "Reader.h"
#include "Writer.h"

using namespace std;

/*****************************************************************************
* This class is a wrapper around a number of system-dependant calls
* to get information about a file
*****************************************************************************/
class File {

private:
    /*******
    * data *
    *******/
    string _path;
    string _base;

public:
    /**************
    * constructor *
    **************/
    File();
    File(const string& path, const string& base);

    /*************
    * destructor *
    *************/
    virtual ~File() {}


    /************
    * accessors *
    ************/
    virtual void path(char* path) { _path = string(path); }
    virtual void base(char* base) { _base = string(base); }

    virtual string name(); 

    virtual File* directory() { return new File(_path.c_str(), ""); }

    /**********
    * methods *
    **********/
    virtual int exists();
    virtual bool directoryExists();
    virtual int size();
    virtual int age();
    virtual int idleTime();

    virtual void makeDirectory(int mode=0777);

    virtual Reader* reader();
    virtual Writer* writer(ios::openmode mode=ios::out|ios::app);
    
    /***************
    * static methods *
    *****************/
    static void list(set<string>& s, const string& pattern);


}; // end of File class

#endif //  FILE_H
