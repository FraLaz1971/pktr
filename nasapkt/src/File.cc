#include "File.h"


#include <fstream>

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <glob.h>
}



/****************************************************************************
* constructor
****************************************************************************/
File::File() {
    _path = "";
    _base = "";
}

/****************************************************************************
* constructor setting file name and path
****************************************************************************/
File::File(const string& path, const string& base) {
    _path = path;
    _base = base;

}





/****************************************************************************
* return the full file name including directory path
****************************************************************************/
string File::name() {

    if(_path == "" ) return _base;

    return _path + string("/") + _base;

}


/****************************************************************************
* returns true if the file exists and false otherwise
****************************************************************************/
int File::exists() {

    struct stat info;

    errno=0;
    int status = stat(name().c_str(), &info);

/*
cout << name()
     << " exists: status="<<status<<" errno="<< errno
     << " ENOENT=" << ENOENT
     << "\n";
*/


    if(status == -1 && errno == ENOENT) return 0;
    else                               return 1;

} // end of exists method


/******************************************************************************
* Returns true if the directory containing this file exists
* This is a shortcut for file->directory()->exists(), and saves memory,
* since this method cleans up the temporary directory object.
******************************************************************************/
bool File::directoryExists() {

    File* dir = this->directory();
    bool result = dir->exists();
    delete dir;
    return result;

} // end of directory_exists method

/****************************************************************************
* returns the size of the file in bytes.
* Returns -1 if the file does not exist or if information is unavailable
****************************************************************************/
int File::size() {

    struct stat info;

    int status = stat(name().c_str(), &info);
    if(status==-1) return -1;

    return info.st_size; 

} // end of size method

/****************************************************************************
* returns the age of the file in seconds since its last modification
****************************************************************************/
int File::age() {

    time_t now = time(NULL);
    if(now == -1 ) return -1;

    struct stat info;
    int status = stat(name().c_str(), &info);
    if(status==-1) return -1;

    return now - info.st_mtime; 

} // end of age method

/****************************************************************************
* returns the time in seconds since the last access to the file
****************************************************************************/
int File::idleTime() {

    time_t now = time(NULL);
    if(now == -1 ) return -1;

    struct stat info;

    int status = stat(name().c_str(), &info);
    if(status==-1) return -1;

    return now - info.st_atime; 

} // end of idleTime method

/****************************************************************************
* create a directory named by this file
****************************************************************************/
void File::makeDirectory(int mode /*=0777*/) {

mkdir(name().c_str(), mode);

} // end of makeDirectory method


/****************************************************************************
* returns a data Reader object for input from this file
****************************************************************************/
Reader* File::reader() {
    return new Reader(new ifstream( name().c_str() ) );
}

/****************************************************************************
* returns a data Writer object for output to this file.
* Files are opened for appending by default.
* returns a NULL pointer if the file cannot be opened.
****************************************************************************/
Writer* File::writer(ios::openmode mode /*=ios::app*/) {

    /***********************
    * try opening the file *
    ***********************/
    ofstream* stream =new ofstream( name().c_str(), mode );
    
    /*********************************
    * make sure the stream opened OK *
    *********************************/
    if(! stream->good() ) {
        cerr << "Could not open "<<name()
             << " eof="<<stream->eof()
             << " bad="<<stream->bad()
             << " fail="<<stream->fail()
             <<endl;

        return NULL;
    }

    /*********************************************
    * wrap a writer around the stream and return *
    *********************************************/
    return new Writer(stream);

} // end of writer method



/****************************************************************************
* returns a list of files matching a template
****************************************************************************/
void File::list(set<string>& s, const string& pattern) {

    glob_t results;

    if(!glob(pattern.c_str(), 0/* flags*/,
             NULL/* error callback function*/, &results)) {
         /******************************************
         * no errors, so copy results into the set *
         ******************************************/

         for(unsigned int i=0; i< results.gl_pathc; ++i) {
             string* file = new string(results.gl_pathv[i]);



             s.insert(s.end(), *file);
         }
     }


     
     globfree(&results);


} // end of glob function



