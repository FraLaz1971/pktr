#include "PacketTee.h"

/***************************************************************************
* give this pipe a set of bit flags describing the packet stream
* entering the pipe.
* This method is called whenever a connection is made
* upstream using the "|" operator. Since it doesn't know which upstream
* source is calling it, it queries the hints from all of the upstream
* sources, bitwise "ands" them and then sends the hints to all the sinks.
***************************************************************************/
void PacketTee::hints(int flags) {

/************************************************************
* we don't know who is giving us these hints so we have
* to query everybody upstream , "&" the results and send
* them downstream to all the sinks 
************************************************************/
flags = hints();

for(vector<PacketPipe*>::iterator it = sinks.begin();
    it != sinks.end();
    ++it ) {

    (*it)->hints(flags);

}


} // end of incoming hints set.

/***************************************************************************
* return a set of bit flags giving information about the packet stream
* leaving this pipe. This method queries all the packet stream hints
* upstream and returns a bitwise "and" of them.
***************************************************************************/
int PacketTee::hints() {

int flags = 0;

for(vector<PacketPipe*>::iterator it = sources.begin();
    it != sources.end();
    ++it ) {
    flags &= (*it)->hints();

}

return flags;

} // end of outgoing hints query



/***************************************************************************
* Override the generic pipe connection method to allow multiple connections.
* This method should not be called directly. Instead the "|" operator
* should be used.
***************************************************************************/
void PacketTee::connectToSource(PacketPipe& source) { 

    sources.insert(sources.end(), &source);
}

/***************************************************************************
* Override the generic pipe connection method to allow multiple connections.
* This method should not be called directly. Instead the "|" operator
* should be used.
***************************************************************************/
void PacketTee::connectToSink(PacketPipe& sink) { 

    sinks.insert(sinks.end(), &sink);
}

/***************************************************************************
* disconnect all the upstream connections
***************************************************************************/
void PacketTee::disconnectFromSource() { 
    sources.erase(sources.begin(), sources.end() ); 
}

/***************************************************************************
* disconnect all the downstream connections
***************************************************************************/
void PacketTee::disconnectFromSink()   { 

    sinks.erase(sinks.begin(), sinks.end() );

}


/**************************************************************************
* Push a copy of the packet into all of the sinks.
**************************************************************************/
void PacketTee::pushIn(CCSDSPacket* p) {

for(vector<PacketPipe*>::iterator it = sinks.begin();
    it != sinks.end();
    ++it ) {

    (*it)->pushIn(p);

}


} // end of pushIn method

/**************************************************************************
* pull a packet from one of the sources. If all the sources are at 
* end of stream then return NULL
**************************************************************************/
CCSDSPacket* PacketTee::pullOut() {

CCSDSPacket* p=NULL;

for(vector<PacketPipe*>::iterator it = sources.begin();
    it != sources.end();
    ++it ) {

    *(*it) >> p;
    if(p!=NULL) return p;
}

return NULL;

} // end of producePacket method

/****************************************************************************
* pull all available packets from all of the sources and push them into all
* of the sinks
****************************************************************************/
void PacketTee::pump() {

CCSDSPacket* p=NULL;

/********************************************
* loop over the packets until we get a null *
********************************************/
do  {

    p = pullOut();
    pushIn(p);

} while(p!=NULL);


} // end of pump method

