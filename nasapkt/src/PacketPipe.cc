#include "PacketPipe.h"

/***************************************************************************
* constructor
***************************************************************************/
PacketPipe::PacketPipe() {
    source=NULL;
    sink  =NULL;

} // end of constructor


/***************************************************************************
* give this pipe a set of bit flags describing the packet stream
* entering the pipe.
* This method just passes this information downstream. Subclasses should
* override this method if they can optimize themselves given any of these
* hints. Note that the hints(0 method gives information about the outgoing
* stream, so a subclass should set internal hints flags when this method is
* called. This method will be called whenever a connection is made
* upstream using operator|
***************************************************************************/
void PacketPipe::hints(int flags) {

if(sink!=NULL) sink->hints(flags);

} // end of incoming hints set.

/***************************************************************************
* return a set of bit flags giving information about the packet stream
* leaving this pipe. This method just queries the packet stream hints
* upstream. Subclasses should override this method if they impose some
* conditions on the stream.
***************************************************************************/
int PacketPipe::hints() {

if(source!=NULL) return source->hints();
else             return 0;

} // end of outgoing hints query




/***************************************************************************
* These connect and dissconnect methods should not be used in general.
* Instead, a pair of pipes should be connected using the "|" operator.
***************************************************************************/
void PacketPipe::connectToSource(PacketPipe& source) { this->source = &source; }
void PacketPipe::connectToSink(  PacketPipe& sink  ) { this->sink   = &sink;   }

void PacketPipe::disconnectFromSource() { this->source=NULL; }
void PacketPipe::disconnectFromSink()   { this->sink  =NULL; }


/**************************************************************************
* Push one packet into the pipe. This method is called by the "<<" operator.
* For this base class, this method just pushes the packet into the next
* pipe downstream if there is one. Subclasses may override this
* method or operateOnPacket() to do something more interesting.
**************************************************************************/
void PacketPipe::pushIn(CCSDSPacket* p) {

//if(sink!= NULL) sink->pushIn(operateOnPacket(p));

    CCSDSPacket* processed = operateOnPacket(p);

    if(sink!= NULL) sink->pushIn(processed);


}

/**************************************************************************
* Pull a packet out of the pipe. This method is called by the ">>" operator.
* For this base class, it just pulls a packet from the upstream pipe
* If there is one. If there isn't, it delivers a NULL pointer indicating
* end-of-stream. Subclasses may override this method or operateOnPacket()
* to do something more interesting.
**************************************************************************/
CCSDSPacket* PacketPipe::pullOut() {

CCSDSPacket* p = NULL;

if(source!=NULL) p = source->pullOut();

return operateOnPacket(p);

} // end of pullOut method

/****************************************************************************
* pull packets from the source 
* and deliver them to the sink until no packets are left. This method is
* useful, e.g. if you have a PacketReader at one end of a chain and
* a PacketWriter at the other end. Then you can call the pump() method
* on some intermediate pipe to process all the packets in some input file.
****************************************************************************/
void PacketPipe::pump() {

CCSDSPacket* p=NULL;

/********************************************
* can't pump anything if there is no source *
********************************************/
if(source==NULL ) {
    *sink << p;
    return;
}

/********************************************
* loop over the packets until we get a null *
********************************************/
do  {

    *source >> p;
    p = operateOnPacket(p);
    if(sink) *sink << p;

} while(p!=NULL);

    

} // end of pump method



/**************************************************************************
* operator for connecting two pipes. Returns a pointer to the sink pipe
* so that many of these may be chained together
**************************************************************************/
PacketPipe& operator|(PacketPipe& source, PacketPipe& sink) {

/************************
* connect the two pipes *
************************/
source.connectToSink(sink);
sink.connectToSource(source);

/**************************************************
* pass packet stream hints information downstream *
**************************************************/
sink.hints(source.hints());

return sink;

} // end of overloaded connection operator


/**************************************************************************
* operator for pulling a packet out of a pipe. See the PullOut() method.
**************************************************************************/
PacketPipe& operator>>(PacketPipe& source, CCSDSPacket* &p) {

//try {
p = source.pullOut();
//} catch(runtime_error& e) {
//    cout << "caught runtime error" << e.what() <<"\n";
//} catch(...) {
//cout << "caught something in pull out\n";
//}


return source;

}

/**************************************************************************
* operator for pushing a packet into a pipe. See the PushIn() method.
**************************************************************************/
PacketPipe& operator<<(PacketPipe& sink, CCSDSPacket* p) {

sink.pushIn(p);

return sink;

}

