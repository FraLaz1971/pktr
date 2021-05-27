#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <string>
#include "CCSDSPacket.h"

class Context {

private:
    /*******
    * data *
    *******/
    CCSDSPacket* p;
    map<string, double> variables;
    ostream* _out;


public:
    /**************
    * constructor *
    **************/
    Context();

    /*************
    * destructor *
    *************/
    virtual ~Context() {}

    /************
    * accessors *
    ************/
    virtual CCSDSPacket* packet();
    virtual void packet(CCSDSPacket* packet);

    virtual void out(ostream* out);
    virtual ostream* out() { return _out; }

    virtual double get(const string& name);
    virtual void   set(const string& name, double value);


    /**********
    * methods *
    **********/

}; // end of Context class

#endif // CONTEXT_H
