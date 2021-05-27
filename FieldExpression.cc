#include "FieldExpression.h"

#include "DataFieldExpression.h"

/*********************************************************************
* constructor
*********************************************************************/
FieldExpression::FieldExpression(const string& name) {
    this->name = name;
} // end of constructor

/*********************************************************************
*
*********************************************************************/
Expression* FieldExpression::eat(istream* in) {


    skipWhiteSpace(in);

    /************************
    * fields start with "%" *
    ************************/
    if(in->peek() != '%' ) return NULL;
    in->get();
    
    string name;
    while(true) {

        char c = in->peek();
        if(c == '[' ) {
            /*********************************
            * this is the start of a bracket *
            **********************************/
            if(name != "U" && name != "I" && name != "F") return NULL;

            int bytes=-1;
            int bits=0;
            int size=-1;

            in->get(); // clear the opening bracket
            skipWhiteSpace(in);
            *in >> bytes;


            if(in->peek() == '.' ) {
                /************************
                * there is a bit offset *
                ************************/
                in->get();
                *in>>bits;
            }

            skipWhiteSpace(in);
            if(in->peek() != ',') return NULL;
            in->get();
            skipWhiteSpace(in);
            *in >> size;


            skipWhiteSpace(in);
            if(in->peek() != ']' ) return NULL;

            if(bytes<0 || size<0) return NULL;
            in->get();

            return new DataFieldExpression(name[0], bytes, bits, size);

        } // end if we encountered a bracket


            

        if(isWordChar(c) ) {
            /****************************
            * this is a valid character *
            ****************************/
            name += c;
            in->get();

        } else break;

    
    }

    if(name == "" ) return NULL;       
    else             return new FieldExpression(name);

} // end of eat method

/*********************************************************************
*
*********************************************************************/
double FieldExpression::evaluate(Context* c) throw(Exception) {

CCSDSPacket* p = c->packet();

if(p==NULL) throw Exception("Null packet\n");

/***********************
* primary header items *
***********************/
if(name == "apid"    ) return (double)p->apid();
else if(name == "first"   ) return (double)p->isFirst();
else if(name == "last"    ) return (double)p->isLast();
else if(name == "sequence") return (double)p->sequence();
else if(name == "length"  ) return (double)p->length();

/***************
* head2 fields *
***************/
else if(name == "time2" ) return asHead2(p)->time();

/***************
* head3 fields *
***************/
else if(name == "obs"        ) return (double)asHead3(p)->obs();
else if(name == "target"     ) return (double)asHead3(p)->target();
else if(name == "time3"      ) return asHead3(p)->sc_time();
else if(name == "correction" ) return asHead3(p)->utcf_time();


/*************
* LDP fields *
*************/
else if(name == "product" ) return (double)asLDP(p)->product();
else if(name == "page"    ) return (double)asLDP(p)->page();

/***************
* packet types *
***************/
else if(name == "head2") {
    if(dynamic_cast<SwiftPacket*>(p) ) return 1.0;
    else                               return 0.0;

} else if(name == "head3") {
    if(dynamic_cast<SwiftHead3Packet*>(p) ) return 1.0;
    else                                    return 0.0;

}else if(name == "ldp" ) {
    if(dynamic_cast<LDPPacket*>(p) ) return 1.0;
    else                             return 0.0;
}


/**********
* unknown *
**********/
else throw Exception("Unknown telemetry field "+name);

} // end of evaluate method

/*********************************************************************
*
*********************************************************************/
void FieldExpression::set(Context* c, double value) throw(Exception) {

CCSDSPacket* p = c->packet();

if(p==NULL) throw Exception("Null packet for assignment\n");

/***********************
* primary header items *
***********************/
if(name == "apid"    ) p->apid((unsigned int)value);
else if(name == "first"   ) p->isFirst((unsigned int)value);
else if(name == "last"    ) p->isLast((unsigned int)value);
else if(name == "sequence") p->sequence((unsigned int)value);
else if(name == "length"  ) p->length((unsigned int)value);

/***************
* head2 fields *
***************/
else if(name == "time2" ) asHead2(p)->time(value);

/***************
* head3 fields *
***************/
else if(name == "obs"        ) asHead3(p)->obs(   (unsigned int)value);
else if(name == "target"     ) asHead3(p)->target((unsigned int)value);
else if(name == "time3"      ) asHead3(p)->sc_time(             value);
else if(name == "correction" ) asHead3(p)->utcf_time(           value);



/*************
* LDP fields *
*************/
else if(name == "product" ) asLDP(p)->product((unsigned int)value);
else if(name == "page"    ) asLDP(p)->page(   (unsigned int)value);

/***************
* packet types *
***************/
else if(name == "head2" || name == "head3" || name == "ldp" )
    throw Exception("Can't assign packet type "+name);




/**********
* unknown *
**********/
else throw Exception("Unknown telemetry field in assignment"+name);


} // end of evaluate method



/*********************************************************************
*
*********************************************************************/
LDPPacket* FieldExpression::asLDP(CCSDSPacket* generic_p) throw(Exception) {

    LDPPacket* p = dynamic_cast<LDPPacket*>(generic_p);

    if(p==NULL) throw Exception("Invalid telemetry field "+name+
                                " for non-LDP packet");

    return p;
}

/*********************************************************************
*
*********************************************************************/
SwiftPacket* FieldExpression::asHead2(CCSDSPacket* generic_p) throw(Exception) {

    SwiftPacket* p = dynamic_cast<SwiftPacket*>(generic_p);

    if(p==NULL) throw Exception("Invalid telemetry field "+name+
                                " for non-Swift packet");

    return p;
}

/*********************************************************************
*
*********************************************************************/
SwiftHead3Packet* FieldExpression::asHead3(CCSDSPacket* generic_p) 
                  throw(Exception) {

    SwiftHead3Packet* p = dynamic_cast<SwiftHead3Packet*>(generic_p);

    if(p==NULL) throw Exception("Invalid telemetry field "+name+
                                " for non-HEAD3 packet");

    return p;
}


