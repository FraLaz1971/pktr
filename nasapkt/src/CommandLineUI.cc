#include "CommandLineUI.h"

/*********************************************************************
* constructor
*********************************************************************/
CommandLineUI::CommandLineUI(int argc, char** argv) {

string* value=NULL;
string* flag=NULL;

for(int i=1; i<argc; ++i) {

    if(argv[i][0]=='-' && argv[i][1]!='\0' ) {
        /*********************************************************
        * this is a flag, signaling the start of a new parameter *
        *********************************************************/
        if(flag!=NULL) {
            /************************************
            * store the previous flag and value *
            ************************************/
            if(value!=NULL) values[*flag] = new String(*value);
            else            values[*flag] = new Boolean(true);
        }

        /********************
        * read the new flag *
        ********************/
        value = NULL;
        flag = new string(argv[i]+1);


    
    } else {
        /************************
        * this is a value field *
        ************************/
        if(value == NULL )  value  = new string(argv[i]);
        else               *value += string(" ")+argv[i];

    }

} // end of loop  over arguments

if(flag!=NULL) {
    /************************************
    * store the previous flag and value *
    ************************************/
    if(value!=NULL) values[*flag] = new String(*value);
    else            values[*flag] = new Boolean(true);

}


} // end of constructor


/*********************************************************************
* pull a given value from the command line arguments
*********************************************************************/
UserInterface::Value* CommandLineUI::extract(const string& key) 
                                     throw(Exception) {


//cout << "requesting key=|"<<key<<"| value=|"<<values[key]->stringValue()<<"|\n";
return values[key];

}


/*********************************************************************
* This is a read-only interface, so setting throws an exception
*********************************************************************/
void CommandLineUI::setValue(const string& key, Value* value) throw(Exception) {

throw Exception("Can't set parameter values in the Command Line Interface.");
}


/*********************************************************************
* Override getBoolean so that a missing flag is treated as a false
* value.
*********************************************************************/
bool CommandLineUI::getBoolean(const string& key) throw(Exception) {

try { return getValue(key)->booleanValue(); }
catch(Exception& e) { return false; }

} // end of getBoolean method
