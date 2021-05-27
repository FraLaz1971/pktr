/****************************************************************************
* read a text file and put each line into the data of a SwiftHead3 packet.
****************************************************************************/

#include <fstream.h>

#include "SwiftHead3Packet.h"
#include "Writer.h"

main() {

/********************
* initialize packet *
********************/
SwiftHead3Packet* p = new SwiftHead3Packet();

p->apid(14);

p->target(1068);
p->obs(1);
p->sc_time(12345.);
p->utcf_time(54321.);


/********************
* initialize writer *
********************/
Writer* w = new Writer(new ofstream("seg_orig.ccsds"));

istream* in = new ifstream("seg.txt");
while(in->good() ) {

    /**************
    * read a line *
    **************/
    char line[1024];
    in->getline(line, 1024, '\n');
    
    int size = strlen(line);
    if(size==0) break;

    cout << "packet size="<<size<<"\n";
    p->size(size);

    unsigned char* d1 = p->data();
    for(int i=0; i<size; ++i) {
        *(d1++) = line[i];
    }

    p->write(w);
    

}


} // end of main
