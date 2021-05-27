#ifndef COMMAND_LINE_UI_H
#define COMMAND_LINE_UI_H

#include "UserInterface.h"

/***************************************************************************
* This class implements a simple command line based user interface.
* A parameter may be specified on the command line as "-keyword value",
* where "keyword" need only be the unique initial characters in the actual
* keyword. 
*
***************************************************************************/
class CommandLineUI : public UserInterface {

private:
    /*****************************************************************
    * Tiny class to compare only existing characters in 
    * two strings. This way command line flags only have to have the
    * minimum numbert of unique characters in them
    ******************************************************************/
    class KeyComparer {
        public:  bool operator()(const string& left, const string& right) {
            int min_length = left.length();
            int right_length = right.length();
            if(right_length<min_length) min_length=right_length;

            return left.substr(0,min_length) < right.substr(0,min_length);
        }
            
    }; // end of KeyComparer embedded class

private:
    /*******
    * data *
    *******/
    map<string, Value*, KeyComparer> values;


public:
    /**************
    * constructor *
    **************/
    CommandLineUI(int argc, char** argv);

    /*************
    * destructor *
    *************/
    virtual ~CommandLineUI() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/

    virtual Value* extract(const string& key) throw(Exception);
    virtual void setValue(const string& key, Value* value) throw(Exception);

    virtual bool getBoolean(const string& key) throw(Exception);


}; // end of CommandLineUI class

#endif // COMMAND_LINE_UI_H
