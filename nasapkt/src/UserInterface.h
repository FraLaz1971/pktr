#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

/***************************************************************************
* This is an abstract base class for a generic user interface.
* It provides a framework for obtaining string or numerical parameters
* by keyword and for setting their defaults.
* It also provides methods for opening [io]streams which can refer to files or 
* stdout/stderr. 
*
* Sub-classes must define the two pure virtual methods which obtain the 
* parameter values from the command line, a parameter file, or some 
* other source.
*
***************************************************************************/
class UserInterface {

public:
    /***********************************************************************
    * exception class
    ***********************************************************************/
    class Exception : public runtime_error {

        public: Exception(const string& context) : runtime_error(context) { }

    }; // end of Exception embedded class


    /**********************************************************************
    * this is an abstract base class representing a value of generic type.
    * Subclasses may represent the data differently, but they all may
    * be accessed in the same way.
    **********************************************************************/
    class Value {

        public:
            virtual double realValue() =0;
            virtual long integerValue() =0;
            virtual bool booleanValue() =0;
            virtual const string& stringValue() =0;
    };

    /**********************************************************************
    * This is a value stored internally as a double
    **********************************************************************/
    class Double : public Value {

        double value;
        string* str;

        public:
            Double(double value) { str=NULL; this->value = value; }
            virtual ~Double() { delete str; str=NULL; }

            virtual double realValue() { return value; }
            virtual long integerValue() { return (long)value; }
            virtual bool booleanValue() { return value != 0.0; }
            virtual const string& stringValue() {

                if(str==NULL) {
                    ostringstream s;
                    s << value;
#                   ifdef FAKING_STRINGSTREAM
		    s <<ends;
#                   endif
                    str = new string(s.str());
#                   ifdef FAKING_STRINGSTREAM
                    delete s.str();
#                   endif
                }
                return *str;
            }
    };

    /**********************************************************************
    * This is a value stored internally as a long
    **********************************************************************/
    class Integer : public Value {

        long value;
        string* str;

        public:
            Integer(long value) { str=NULL; this->value = value; }
            virtual ~Integer() { delete str; str=NULL; }

            virtual double realValue() { return (double)value; }
            virtual long integerValue() { return value; }
            virtual bool booleanValue() { return value != 0; }
            virtual const string& stringValue() {

                if(str==NULL) {
                    ostringstream s;
                    s << value;
#                   ifdef FAKING_STRINGSTREAM
		    s << ends;
#                   endif
                    str = new string(s.str());
                }
                return *str;
            }
    };

    /**********************************************************************
    * This is a value stored internally as a long
    **********************************************************************/
    class String : public Value {

        const string* value;

        public:
            String(const string& value) { this->value = &value; }
            String(const char* value) { this->value = new string(value); }

            virtual double realValue() { 
                istringstream s(value->c_str());
                double real;
                s >> real;
                if(s.fail()) throw Exception("Can't parse real number from "
                                              + *value);

                return real;
            }

            virtual long integerValue() { 
                istringstream s(value->c_str());
                long integer;
                s >> integer;
                if(s.fail()) throw Exception("Can't parse integer from "
                                              + *value);
                return integer;
            }

            virtual bool booleanValue() {

                if(*value == "true" || *value == "True" || *value == "TRUE" ||
                   *value == "yes"  || *value == "Yes"  || *value == "YES" ||
                   *value == "t" || *value == "T" || 
                   *value == "y" || * value =="Y") {

                    return true;
                }

                if(*value == "false" || *value == "False" || *value == "FALSE"||
                   *value == "no"    || *value == "No"    || *value == "NO" ||
                   *value == "f" || *value == "F" || 
                   *value == "n" || * value =="N") {

                    return false;
                }

                throw Exception("Can't parse boolean from "+ *value);
            }

            virtual const string& stringValue() { return *value; }
    };

    /**********************************************************************
    * This is a value stored internally as a bool
    **********************************************************************/
    class Boolean : public Value {

        bool value;
        string* str;

        public:
            Boolean(bool value) { this->value = value; str=NULL; }
            virtual ~Boolean() { delete str; str=NULL; }


            virtual double realValue() { 
                if(value) return 1.0;
                else      return 0.0;
            }

            virtual long integerValue() { 
                if(value) return 1;
                else      return 0;
            }

            virtual bool booleanValue() { return value; }


            virtual const string& stringValue() {

                if(str==NULL) {
                    if(value) str = new string("true");
                    else      str = new string("false");
                }
                return *str;

            
            }
    };




private:
    /*******
    * data *
    *******/
    map<string,Value*> defaults;
    vector<ios*> streams;

public:
    /**************
    * constructor *
    **************/
    UserInterface();

    /*************
    * destructor *
    *************/
    virtual ~UserInterface();

    /************
    * accessors *
    ************/
    virtual void setDefaultValue(const string& key, Value* value);
    virtual void setDefault(const string& key, double value);
    virtual void setDefault(const string& key, int value);
    virtual void setDefault(const string& key, long value);
    virtual void setDefault(const string& key, bool value);
    virtual void setDefault(const string& key, const string& value);
    virtual void setDefault(const string& key, const char* value);

    

    /**********
    * methods *
    **********/
    virtual Value* getValue(const string& key) throw(Exception);
    virtual double getReal(const string& key) throw(Exception);
    virtual long getInteger(const string& key) throw(Exception);
    virtual bool getBoolean(const string& key) throw(Exception);
    virtual const string& getString(const string& key) throw(Exception);

    virtual istream* getIstream(const string& key,
                                ios::openmode mode=ios::in|ios::binary)
                     throw(Exception);
    virtual ostream* getOstream(const string& key, 
                                ios::openmode mode=ios::out|ios::binary) 
                     throw(Exception);


    virtual Value* extract(const string& key) throw(Exception) =0;
    virtual void setValue(const string& key, Value* value) throw(Exception) =0;

    virtual void set(const string& key, double value) throw(Exception);
    virtual void set(const string& key, long value) throw(Exception);
    virtual void set(const string& key, bool value) throw(Exception);
    virtual void set(const string& key, const string& value) throw(Exception);

    


 }; // end of UserInterface class


#endif // USER_INTERFACE_H
