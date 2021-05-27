/*****************************************************************************
*
* DESCRIPTION: This tool allows you to filter packets out of a data stream
* based on an expression involving header values. Only packets for which the
* expression evaluates as true will be written to the output file.
* The expression may contain the following special symbols:
* - apid = The packet APID
* - first = The primary header flag which is 1 for the first packet in a group.
* - last = The primary header flag which is 1 for the last packet in a group.
* - sequence = the packet sequence counter in the primary header
* - length   = the length field in the primary header
*
* For LDP packets the following symbols may also be used:
*
* - product = The LDP product number
* - page    = The LDP page number
*
* Here are some examples:
*
* - "apid != 2045 && apid != 2046 && apid != 2047" - removes all packets with 
*   APIDS 2045, 2046, and 2047
* - "apid == 1344 && product == 234" - retains all packets of XRT LDP 
*   product number 234
*
* PARAMETERS:
*
* - infile The input file name. 
* - outfile the output file name. The output file should not be the
*   same as the input file.
* - apid_list The name of a configuration file for the SwiftTypedPacketReader
*   which lists the packet types for various APIDs.
* - expression this is an expression using C languange syntax describing the
*   packets you would like to retain in the stream. You may use all the
*   familiar operators such as >, <, ==, !=, &&, ||, !, +, -, *, /, (, ), etc.
*   All numbers are treated as double precision floating point with
*   0.0 corresponding to "false" and all other numbers corresponding to "true".
*   See above for more information and examples.
*
******************************************************************************/

#ifndef DEFAULT_APID_LIST
#define DEFAULT_APID_LIST "swift_apids.list"
#endif

#include <fstream>
#include <string>
#include <sstream>

#include "SwiftTypedPacketReader.h"
#include "PacketTee.h"
#include "PacketFilter.h"
#include "PacketWriter.h"
#include "File.h"


/*******************************
* emergency exception handlers *
*******************************/
void my_unexpected() {
    cerr << "Unexpected exception thrown\n";
    exit(1);
}

void my_terminate() {
    cerr << "Terminating - probably from uncaught exception\n";
    exit(1);
}


/***************************************************************************
*
***************************************************************************/
void usage_error() {


    cerr << "Usage: squirt [options] script arg1 arg2...\n";
    cerr << "Options:\n";
    cerr << "-i infile    read packets from this file instead of stdin\n";
    cerr << "-o outfile   write packets to this file instead of stdout\n";
    cerr << "-a apid_list use this APID list intead of the default\n";
    cerr << "-e commands  execute the given commands intead of a file\n";
    cerr << "-v           trace the script as it is parsed and executed\n";


    exit(1);

} // end of usage_error function

/***************************************************************************
*
***************************************************************************/
void trim_leading_whitespace(string& line) {

    for(int i=0; i<(int)line.length(); ++i) {
        char c = line[i];
        if(c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            /*********************
            * end of white space *
            *********************/
            if(i>0) line.erase(0, i);
            break;
        }
    }
}



    

/***************************************************************************
*
***************************************************************************/
int main(int argc, char** argv) {

/***********************************
* set emergency exception handlers *
***********************************/
set_unexpected(my_unexpected);
set_terminate(my_terminate);


/************************
* read the command line *
************************/
if(argc==1) usage_error();

/***********
* defaults *
***********/
istream* filter_in  = &cin;
ostream* filter_out = &cout;
istream* script_in = NULL;
string   apid_list = DEFAULT_APID_LIST;
bool verbose = false;

/**************************
* read the flag arguments *
**************************/
int arg=1;
while(arg<argc ) {

    string flag = argv[arg];

    if(flag[0] == '-' ) {
        /*************************
        * this is an option flag *
        *************************/
        if(flag == "-i") {
            /*********
            * infile *
            *********/
            if(++arg==argc) usage_error();
            filter_in = new ifstream(argv[arg++]);
            
        } else if(flag == "-o" ) {
            /**********
            * outfile *
            **********/
            if(++arg==argc) usage_error();
            filter_out = new ofstream(argv[arg++]);

        } else if(flag == "-a" ) {
            /************
            * APID list *
            ************/
            if(++arg==argc) usage_error();
            apid_list = argv[arg++];


        } else if(flag == "-e" ) {
            /**********************
            * command line script *
            **********************/
            if(++arg==argc) usage_error();
            script_in = new istringstream(argv[arg++]);

        } else if(flag == "-v" ) {
            /**********************
            * command line script *
            **********************/
            verbose=true;
            ++arg;

        } else {
            /***************
            * unknown flag *
            ***************/
            usage_error();
        }

    } else {
        /**************************
        * first non-flag argument *
        **************************/
        break;
    }

} // end of loop over args

/*************************************************
* check to be sure the APID list is a valid file *
*************************************************/
File file(apid_list.c_str(), "");
if(!file.exists() ) {
    cerr << "APID list file "<<apid_list<<" does not exist\n";
    exit(1);
}


/*********************************
* other options for script input *
*********************************/
if(!script_in) {
    /**************************************
    * need to read the script from a file *
    **************************************/
    if(arg==argc) usage_error();
    script_in = new ifstream(argv[arg++]);
}

if(script_in == NULL) usage_error();


/***********************
* now parse the script *
***********************/
int buffer_length=256;
char* buffer = new char[256];

vector<Expression*> init;
vector<Expression*> calc;
Expression* filter_expression = NULL;
vector<Expression*> after;

try { // try block to catch parsing exceptions

string line = "";
while(script_in->get(buffer, buffer_length, ';') ){

    /******************************************
    * append the buffer onto the current line *
    ******************************************/
    line += buffer;

//cout <<"\n-----------\n";
//cout << "buffer="<<buffer<<"\n";
//cout << "line="<<line<<"\n";

    if(script_in->peek() != ';' ) {
        /*****************************************
        * the line did not all fit in the buffer *
        *****************************************/
        continue;
    }

    /***************************************************
    * if we get here then we have read a complete line *
    ***************************************************/
    script_in->get(); // clear the semicolon

    /******************************************
    * delete leading whitespace from the line *
    ******************************************/
    trim_leading_whitespace(line);


    /*****************************************************
    * ignore comments
    * comments are terminated at the end of the line or
    * by a semicolon
    *****************************************************/
    while(line[0] == '#') {
        int pos = line.find("\n", 0);
        if(pos != (int)string::npos) {
            line.erase(0, pos+1);
            trim_leading_whitespace(line);
        }
    }

    /*******************
    * skip blank lines *
    *******************/
    if(line == "" ) continue;

    /**********************************
    * check what type of line this is *
    **********************************/
    int pos = line.find(":", 0);
    if(pos == (int)string::npos) {
        /**********************************************
        * no colon in this line - must be a calc line *
        **********************************************/
        if(verbose) cerr << "PARSING "<<line<<";\n";
        calc.insert(calc.end(), Expression::parse(line) );
    } else {
        /*********************************
        * this line has a type specifier *
        *********************************/
        string type = line.substr(0, pos);
        line.erase(0, pos+1);

        if(verbose) cerr << "PARSING "<<type<<": "<<line<<";\n";
        if(type == "init" ) {
            /*******
            * init *
            *******/
            init.insert(init.end(), Expression::parse(line) );

        } else if(type == "after") {
            /********
            * after *
            ********/
            after.insert(after.end(), Expression::parse(line) );

        } else if(type == "filter") {
            /*********
            * filter *
            *********/
            filter_expression = Expression::parse(line);

        } else {
            /***************
            * unknown type *
            ***************/
            throw Expression::Exception("Unknown expression type "+type);
        }
    }

    /************************************************
    * empty out the line now that we have parsed it *
    ************************************************/
    line="";
        

} // end of loop over lines

delete script_in;
delete buffer;

} catch(Expression::Exception& e ) {
    /*************************
    * caught a parsing error *
    *************************/
    cerr << "Error: "<<e.what()<<"\n";
    exit(1);
}

/***************************
* set up the packet filter *
***************************/
PacketFilter filter;
filter.setInit(init);
filter.setCalc(calc);
filter.setAfter(after);
filter.setFilter(filter_expression);
filter.setVerbose(verbose);

/**********************************************
* now set the command line argument variables *
**********************************************/
Context* context = filter.getContext();

for(int i=1; arg<argc; ++arg, ++i) {
  
    ostringstream format;
    format << i << ends;
    string name = format.str();
    name = name.c_str(); // unfortunate cludge to get rid of "ends"

    istringstream unformat(argv[arg]);
    double value;
    unformat >> value;

    context->set(name, value);
}



    
/******************
* create a reader *
******************/
SwiftTypedPacketReader reader(apid_list, filter_in);

/*******************************
* merge all the reader outputs *
*******************************/
PacketTee tee;

reader.head2()   | tee;
reader.head3()   | tee;
reader.uvot()    | tee;
reader.ldp()     | tee;
reader.acs()     | tee;
reader.generic() | tee;

/********************************
* now the filter and the writer *
********************************/
PacketWriter writer(filter_out);

tee  | filter | writer;

/*****************
* now make it go *
*****************/
reader.pump();


if(filter_out != &cout ) delete filter_out;
if(filter_in  != &cin  ) delete filter_in ;



} // end of main
