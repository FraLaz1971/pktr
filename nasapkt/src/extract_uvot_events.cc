/*****************************************************************************
*
* DESCRIPTION: This tool extracts uncompressed UVOT event data and 
* writes them in plain ASCII values.
*
* PARAMETERS:
* - infile The file from which to read packets
* - outfile The file to which the events are written
* - event_apid APID containing event data. All other APIDs are ignored
*
******************************************************************************/

#include "CommandLineUI.h"
#include "PacketReader.h"
#include "SwiftHead3Packet.h"


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
int main(int argc, char** argv) {

/***********************************
* set emergency exception handlers *
***********************************/
set_unexpected(my_unexpected);
set_terminate(my_terminate);


try { // big try block around the entire program
UserInterface* ui = new CommandLineUI(argc, argv);

ui->setDefault("infile","-");
ui->setDefault("outfile","-");
ui->setDefault("event_apid", 771);


int event_apid = ui->getInteger("event_apid");


PacketReader reader(ui->getIstream("infile"), new SwiftHead3Packet() );

ostream& out = *(ui->getOstream("outfile"));


while(true) {


    CCSDSPacket* generic_p;
    reader >> generic_p;
    if(generic_p==NULL) break;


    if((int)generic_p->apid() == event_apid) {
        /**********************
        * merged event packet *
        **********************/
        SwiftHead3Packet* p = dynamic_cast<SwiftHead3Packet*>(generic_p);
        if(p==NULL) {
            cerr << "Couldn't cast UVOT event packet as a SwiftHead3Packet\n";
            exit(1);
        }

        double time = p->sc_time();
        Reader* r = p->dataReader();

        r->readUnsignedInt(2); // exposure ID
        int count       = r->readUnsignedInt(2);

        for(int i=0; i<count; ++i) {
            int x = r->readUnsignedInt(2);
            int y = r->readUnsignedInt(2);

            out << time <<" "<< x <<" "<< y <<"\n";
        }

        delete r;

    } // end if this is a merged UVOT event packet

        


} // end of loop over packets



/******
* end *
******/
delete ui;
exit(0);

} catch(UserInterface::Exception& e) {
    cerr << "User Interface Exception:\n";
    cerr << e.what()<<"\n";
    exit(1);
} catch(Interpreter::Exception& e) {
    cerr << "I/O Exception:\n";
    cerr << e.what()<<"\n";
    exit(1);
}


} // end of main
