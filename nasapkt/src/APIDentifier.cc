#include "APIDentifier.h"
#include <fstream>
#include <sstream>

/*****************************************************************************
* constructor 
*****************************************************************************/
APIDentifier::APIDentifier(const string& file) {

int length=1024;
char line[length];

ifstream in(file.c_str());

while(in.getline(line,length).good() ) {

    /*****************
    * parse the line *
    *****************/
    istringstream parse(line);

    parse >> ws; // eat leading white space
    if(parse.peek() == '#' ) continue; // skip comment lines

    int apid;
    string type;
    parse >> apid >> type;
    if(!parse.good() ) continue; // skip blank and nonsensical lines

    /**************************************
    * add the result to the internal list *
    **************************************/
//    cout << "apid="<<apid <<" type="<<type
//         <<" code="<<parseTypeString(type)<<"\n";

    types[apid] = parseTypeString(type);
    

} // end of loop over lines

} // end of constructor


/*****************************************************************************
* parse a string representation of a type and return its integer code 
*****************************************************************************/
int APIDentifier::parseTypeString(string word) {


if(word == "head2"    ) return HEAD2;
if(word == "head3"    ) return HEAD3;
if(word == "uvot"     ) return UVOT;
if(word == "ldp"      ) return LDP;
if(word == "checksum" ) return CHECKSUM;
if(word == "acs"      ) return ACS;
if(word == "tdrss"    ) return TDRSS;
if(word == "trash"    ) return TRASH;

return UNKNOWN;

} // end of parseTypeString method
