#include "PacketFilter.h"

/*********************************************************************
* constructor
*********************************************************************/
PacketFilter::PacketFilter() {

    context = new Context();
    context->out(&cerr);

    init_done = false;
    after_done=false;

} // end of constructor

/*********************************************************************
* destructor
*********************************************************************/
PacketFilter::~PacketFilter() { 

    delete context;
}

/*********************************************************************
*
*********************************************************************/
bool PacketFilter::handlePacket(CCSDSPacket* p) throw(Expression::Exception) {

//cerr << "processing packet "<<p<<"\n";

    /*******************************************
    * set the packet in the evaluation context *
    *******************************************/
    context->packet(p);

    /***********************************
    * initialize if we haven't already *
    ***********************************/
    if(! init_done) {

        execute(init);
        init_done = true;

    } // end if we need to initialize


    if(p==NULL ) {
        /*******************
        * at end of stream *
        *******************/
        if(!after_done) execute(after);
        after_done=true;
        return true;
    }

    /************************************
    * evaluate all the calc expressions *
    ************************************/
    execute(calc);

    /**************************************
    * evaluate the filter if there is one *
    **************************************/
    if(filter) {
        if(verbose) cerr << "FILTERING: "<<filter<<"\n";
        return filter->evaluate(context) != 0.0;

    } else {
        /******************************
        * no filter - pass everything *
        ******************************/
        return true;
    }



} // end of handlePacket method

/*********************************************************************
*
*********************************************************************/
void PacketFilter::pushIn(CCSDSPacket* p) {

    try {
        if(handlePacket(p) && sink != NULL ) *sink << p;
    } catch(Expression::Exception& e) {
        report(e);
        if(sink != NULL) *sink << (CCSDSPacket*)NULL;
    }
    

}

/*********************************************************************
*
*********************************************************************/
CCSDSPacket* PacketFilter::pullOut() {

    if(source==NULL) return NULL;

    try {

        while(true) {

            CCSDSPacket* p;
            *source >> p;
            if(handlePacket(p) ) return p;
        }
    } catch(Expression::Exception& e) {
        report(e);
        return NULL;
    }

} // end of pullOut method

/*********************************************************************
* execute a set of expressions. Note the packet  is assumed to have
* already been set in the context
*********************************************************************/
void PacketFilter::execute(vector<Expression*>& v) throw(Expression::Exception) {

    for(vector<Expression*>::iterator it = v.begin(); 
        it != v.end(); ++it) {

        Expression* e = *it;
        if(verbose) cerr << "EXECUTING: "<<e<<"\n";

        e->evaluate(context);
    }

} // end of execute method

/*********************************************************************
*
*********************************************************************/
void PacketFilter::report(Expression::Exception& e) {

    cerr << "Error["<<context->get("packet")<<"]: "<<e.what()<<"\n";
}
