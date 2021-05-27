
#include "LDPPacket.h"
#include "SwiftHead3Packet.h"
#include "LDPPacketBinner.h"
#include "Head3PacketBinner.h"


void my_unexpected() {
    cerr << "Unexpected exception thrown\n";
    exit(1);
}

void my_terminate() {
    cerr << "Terminating\n";
    exit(1);
}




//------------------------------------------------------------------
main(int argc, char** argv) {

Head3PacketBinner binner;
//SwiftHead3Packet* packet = new SwiftHead3Packet();

for(int i=0; i<10000000; ++i) {

//File* file =  binner.getFile(packet);
File* file =  binner.getFileNearTime(10, 100, (unsigned int)1000);
delete file;
}



exit(0);



} // end of main
