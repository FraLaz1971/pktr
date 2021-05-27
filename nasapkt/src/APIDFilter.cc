#include "APIDFilter.h"

#include <sstream>

/*********************************************************************
* constructor
*********************************************************************/
APIDFilter::APIDFilter() {
} // end of constructor


/*********************************************************************
* destructor - delete all the interval objects
*********************************************************************/
APIDFilter::~APIDFilter() {

    for(vector<Interval*>::iterator it=intervals.begin(); 
        it != intervals.end(); ++it) {

        Interval* interval = *it;

        delete interval;
    }


} // end of destructor


/*********************************************************************
*
*********************************************************************/
void APIDFilter::addInterval(int from, int to) {

    intervals.insert(intervals.end(), new Interval(from, to) );

} // end of addInterval method

/*********************************************************************
*
*********************************************************************/
void APIDFilter::addIntervals(const string& list) {

    /*********************************************
    * break the list into comma-separated tokens *
    *********************************************/
    string::size_type from=0;
    for(string::size_type to=0; to < list.length(); from=to+1) {

        /****************************
        * extract the current token *
        ****************************/
        to = list.find(",", from);
        if(to==string::npos) to = list.length();

        string token = list.substr(from, to-from);

        /******************
        * parse the token *
        ******************/
        int apid1=-1;
        int apid2=-1;
        string::size_type dash = token.find("-", 0);
        if(dash==string::npos) {
            /*************************************
            * no dash - treat as a single number *
            *************************************/
            istringstream parse(token.c_str());
            parse >> apid1;
            apid2 = apid1;
        } else {
           /************************************
           * contains a dash, treat as a range *
           ************************************/
           string left = token.substr(0, dash);
           istringstream parse_left(left.c_str());
           parse_left >> apid1;

           string right = token.substr(dash+1, token.length()-dash);
           istringstream parse_right(right.c_str());
           parse_right >> apid2;

        }

        addInterval(apid1, apid2);



        cout << " |"<< token<<"|"
             << "apid1="<<apid1<<" apid2="<<apid2<<"\n";

    } // end of loop over tokens

} // end of addIntervals method

/*********************************************************************
*
*********************************************************************/
bool APIDFilter::accept(CCSDSPacket* p) {

    if(p==NULL) return true;

    int apid = p->apid();
    
   // cout << "APIDFilter::accept apid="<<apid<<endl;

    for(vector<Interval*>::iterator it=intervals.begin();
        it != intervals.end(); ++it) {

        Interval* interval = *it;

        if(interval->contains(apid) ) {
          //  cout << "accepted"<<endl;
            return true;
        }

    }
    
  //  cout << "rejected"<<endl;

    return false;


} // end of accept method
