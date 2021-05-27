#include "UserInterface.h"
#include <fstream>

/*********************************************************************
* constructor
*********************************************************************/
UserInterface::UserInterface() {

} // end of constructor

/*********************************************************************
* destructor - delete temporary storage
*********************************************************************/
UserInterface::~UserInterface() {

/********************************
* delete any streams we created *
********************************/
for(vector<ios*>::iterator it=streams.begin(); it != streams.end(); ++it ) {

    ofstream* file = dynamic_cast<ofstream*>(*it);
    if(file!= NULL) { 
        //cout << "Closing ofstream "<<file<<endl;
        file->close();
    } else {
      //  cout << "destructing "<<*it<<endl;
    }

    delete dynamic_cast<istream*>(*it);
    delete dynamic_cast<ostream*>(*it);
}

streams.clear();


} // end of constructor


/*********************************************************************
* set a default value for a given keyword
*********************************************************************/
void UserInterface::setDefaultValue(const string& key, Value* value) {
    defaults[key] = value;
}

/*********************************************************************
* set a default value for a given keyword
*********************************************************************/
void UserInterface::setDefault(const string& key, double value) {

    defaults[key] = new Double(value);

}

/*********************************************************************
* set a default value for a given keyword
*********************************************************************/
void UserInterface::setDefault(const string& key, int value) {

    defaults[key] = new Integer((long)value);

} 


/*********************************************************************
* set a default value for a given keyword
*********************************************************************/
void UserInterface::setDefault(const string& key, long value) {

    defaults[key] = new Integer(value);

} 

/*********************************************************************
* set a default value for a given keyword
*********************************************************************/
void UserInterface::setDefault(const string& key, bool value) {

    defaults[key] = new Boolean(value);

} 


/*********************************************************************
* set a default value for a given keyword
*********************************************************************/
void UserInterface::setDefault(const string& key, const string& value) {

    defaults[key] = new String(value);

} 

/*********************************************************************
* set a default value for a given keyword
*********************************************************************/
void UserInterface::setDefault(const string& key, const char* value) {

    defaults[key] = new String(value);

} 




/*********************************************************************
* return a Value object containing the value associated with the
* given key. First it tries to extract the value in an interface dependant
* way. If this fails, it returns the default value, if there was one.
* If that fails it returns a NULL pointer.
*********************************************************************/
UserInterface::Value* UserInterface::getValue(const string& key) 
                                     throw(Exception){

    Value* value = extract(key);
    if(value == NULL)  value = defaults[key];
    if(value == NULL) throw Exception("Cannot get value for "+key);

    return value;
            
} // end of get method


/*********************************************************************
* return the value associated with a key interpreted as a double.
*********************************************************************/
double UserInterface::getReal(const string& key) throw(Exception) {

    return getValue(key)->realValue();
}

/*********************************************************************
* return the value associated with a key interpreted as an integer
*********************************************************************/
long UserInterface::getInteger(const string& key) throw(Exception) {

    return getValue(key)->integerValue();
}

/*********************************************************************
* return the value associated with a key interpreted as a bool
*********************************************************************/
bool UserInterface::getBoolean(const string& key) throw(Exception) {

    return getValue(key)->booleanValue();
}


/*********************************************************************
* return the value associated with a key interpreted as an integer
*********************************************************************/
const string& UserInterface::getString(const string& key) throw(Exception) {

    return getValue(key)->stringValue();
}


/*********************************************************************
* Get an istream corresponding to the file named by a given keyword value.
* A file named "-" is considered to mean stdin (i.e. cin).
* Note that you should NOT delete this istream when you are done
* with it. The UserInterface will take care of that in its destructor.
* The reason is that the stream might actually be "cin" and deleting
* That can cause a segmentation fault.
*********************************************************************/
istream* UserInterface::getIstream(const string& key, 
                                   ios::openmode mode/*=ios::in|ios::binary*/) 
                        throw(Exception) {

    string name = getString(key);
    if(name == "-") return &cin;

    istream* stream = new ifstream(name.c_str(), mode);
    if( ! stream->good() ) {
        throw Exception("Could not open file "+name+" for reading");
    }

    streams.insert(streams.end(), stream);

    return stream;
}

/*********************************************************************
* Same as getIstream, except it returns an ostream, and "-" refers
* to stdout (cout).
*********************************************************************/
ostream* UserInterface::getOstream(const string& key, 
                                 ios::openmode mode/*=ios::out|ios::binary*/) 
                        throw(Exception) {


    string name = getString(key);
    if(name == "-") return &cout;

    ostream* stream = new ofstream(name.c_str(), mode);
    streams.insert(streams.end(), stream);
//cout << " created "<<stream<<" for "<<key<<" = "<<name<<endl;
//for(vector<ios*>::iterator it=streams.begin(); it != streams.end(); ++it ) {
//    cout << "    remembered stream: "<< *it<<endl;
//}
    return stream;
}




/*********************************************************************
* set a value for a given keyword
*********************************************************************/
void UserInterface::set(const string& key, double value) 
                    throw(Exception){

    setValue(key, new Double(value) );
}

/*********************************************************************
* set a value for a given keyword
*********************************************************************/
void UserInterface::set(const string& key, long value) throw(Exception){

    setValue(key, new Integer(value) );
} 

/*********************************************************************
* set a value for a given keyword
*********************************************************************/
void UserInterface::set(const string& key, bool value) throw(Exception){

    setValue(key, new Boolean(value) );
} 


/*********************************************************************
* set a value for a given keyword
*********************************************************************/
void UserInterface::set(const string& key, const string& value) 
                    throw(Exception){

    setValue(key, new String(value) );
} 

