#ifndef PACKET_FILTER_H
#define PACKET_FILTER_H

#include "PacketPipe.h"
#include "Expression.h"

class PacketFilter : public PacketPipe {

private:
    /*******
    * data *
    *******/
    bool verbose;
    bool init_done;
    bool after_done;
    Context* context;
    vector<Expression*> init;
    vector<Expression*> calc;
    vector<Expression*> after;
    Expression* filter;

public:
    /**************
    * constructor *
    **************/
    PacketFilter();

    /*************
    * destructor *
    *************/
    virtual ~PacketFilter();

    /************
    * accessors *
    ************/
    Context* getContext() { return context; }
    void setInit( vector<Expression*>& v) { init  = v; }
    void setCalc( vector<Expression*>& v) { calc  = v; }
    void setAfter(vector<Expression*>& v) { after = v; }
    void setFilter(Expression* e) { filter = e; }

    void setVerbose(bool verbose) { this->verbose = verbose; }
    

    /**********
    * methods *
    **********/
    virtual void pushIn(CCSDSPacket* p);
    virtual CCSDSPacket* pullOut();

protected:
    virtual bool handlePacket(CCSDSPacket* p) throw(Expression::Exception);
    virtual void execute(vector<Expression*>& v) throw(Expression::Exception);

    virtual void report(Expression::Exception& e);


}; // end of PacketFilter class

#endif // PACKET_FILTER_H
