#include "Context.h"

#include <iomanip>


/*********************************************************************
* constructor
*********************************************************************/
Context::Context() {

    /******************************
    * initialize the packet count *
    ******************************/
    set("packet", 0.0);

} // end of constructor

/*********************************************************************
*
*********************************************************************/
CCSDSPacket* Context::packet() { return p; }

/*********************************************************************
* set the current packet and increment the packet count
*********************************************************************/
void Context::packet(CCSDSPacket* p) { 

    this->p = p;
    set("packet", get("packet")+1);
}

/*********************************************************************
* get the value of a variable
*********************************************************************/
double Context::get(const string& name) { return variables[name]; }

/*********************************************************************
* set the value of a variable
*********************************************************************/
void Context::set(const string& name, double value) { variables[name]=value; }


/*********************************************************************
* sets the ostream which will be used for printing strings.
* also sets the floating point precision on that stream.
*********************************************************************/
void Context::out(ostream* out) { 
    _out = out; 
    *_out << setprecision(14);
}
