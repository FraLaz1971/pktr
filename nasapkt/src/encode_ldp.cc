/*****************************************************************************
*
* DESCRIPTION: This tool takes arbitrary data and encodes it into a single
* Swift large data product. 
*
* PARAMETERS:
* - infile  The data to be encoded in the LDP are taken from this file.
*
* - outfile File to contain the encoded LDP packets.
*
* - checksum Should each LDP packet include a checksum? Checksums are optional,
*   but both the BAT and XRT have decided to use them.
*
* - size The maximum number of bytes to include in the data part of each packet.
*   Note that this does not include the headers, product number, page number,
*   or checksum.
*
* - apid This is the APID to be used in all the packets in decimal. 
*   The XRT will use 1344 for all it's LDPs. The BAT will (probably) use
*   299 for real-time engineering data, 
*   481 for recorded engineering data,
*   527 for burst data, and
*   528 for survey data.
*
* - product The product number identifying this product. This must fit in 
*   a two byte unsigned unteger.
*
* - sequence The sequence counter value to use in the CCSDS primary header
*   of the first packet.
*
* - time The time in seconds to use in the standard part of the LDP header as 
*   well as in the secondary header of the first packet. In general these will 
*   not be same, but this program assumes they are for simplicity.
*
* - delta_time The interval in seconds between the secondary header times of
*   consecutive packets. In reality these times will not increment so regularly
*   but this program assumes they do for simplicity.
*
* - ntarget The target ID number This currently must fit in a three byte 
*   unsigned integer.
*
* - nobs The observation counter. This currently must fit in a one byte
*   unsigned integer. The current plan is for this counter to start at 1.
*
* - extra_header_size The Header of an LDP contains a standard part followed
*   by a non-standard part. This program generates the standard part and then
*   assumes that the first "extra_header_size" bytes of the input file make
*   up the non-standard header. 
* 
****************************************************************************/
#include "LDPEncodeBuffer.h"
#include "ChecksumLDPPacket.h"
#include "PacketWriter.h"
#include "CommandLineUI.h"
#include "RawDataBlock.h"
#include <fstream>

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
int main(int argc, char* argv[] ) {

/***********************************
* set emergency exception handlers *
***********************************/
set_unexpected(my_unexpected);
set_terminate(my_terminate);


try { // big try block around the entire program
UserInterface* ui = new CommandLineUI(argc, argv);

/*************
* parameters *
*************/
ui->setDefault("checksum", true);
ui->setDefault("size", 940);
ui->setDefault("apid", 1344);
ui->setDefault("product",0);
ui->setDefault("sequence", 0);
ui->setDefault("time", 0.);
ui->setDefault("delta_time",0.05);
ui->setDefault("ntarget",0);
ui->setDefault("nobs",1);
ui->setDefault("extra_header_size",0);

ui->setDefault("infile", "-");
ui->setDefault("outfile", "-");


bool has_checksum     = ui->getBoolean("checksum");
int packet_data_size  = ui->getInteger("size");

int apid              = ui->getInteger("apid");
int product           = ui->getInteger("product");

int sequence0         = ui->getInteger("sequence");

double time0          = ui->getReal("time");
double time_increment = ui->getReal("delta_time");

int target            = ui->getInteger("ntarget");
int obs               = ui->getInteger("nobs");

int extra_header_size = ui->getInteger("extra_header_size");

/*************************************************************
* test to be sure the Writer is compatible with the hardware *
*************************************************************/
try {
    Writer::checkMachine();
} catch(Reader::HardwareException e) {
    cerr << "Hardware exception:" << e.what() << "\n";
    exit(1);
}



/**************************
* set up the first packet *
**************************/
LDPPacket* p;
if(has_checksum) p = new ChecksumLDPPacket();
else             p = new         LDPPacket();

p->apid(apid);
p->product(product);
p->time(time0);
p->sequence(sequence0);
p->size(packet_data_size);

/**********************
* make the LDP header *
**********************/
StandardLDPHead* head = new StandardLDPHead();

head->npages(0);

SwiftHead3* head3 = head->head3();
head3->target(target);
head3->obs(obs);
head3->sc_time()->value(time0);
head3->utcf_time()->value(time0);


/*****************************
* make the input data stream *
*****************************/
istream* in = ui->getIstream("infile");

/************************************************
* now create and fill the extra header - if any *
************************************************/
RawDataBlock* extra_header = NULL;
if(extra_header_size >0) {
    unsigned char* data = new unsigned char[extra_header_size];
    in->read((char*)data, extra_header_size);

    extra_header = new RawDataBlock(data, extra_header_size);
}
    

/******************************************************************
* create the encoder and the packet writer and hook them together *
******************************************************************/
LDPEncodeBuffer*  ldp = new LDPEncodeBuffer(p, head, extra_header, 
                                            time_increment);

PacketWriter writer(ui->getOstream("outfile") );


ldp->pipe() | writer;


/*********************************************
* create a data stream around the ldp buffer *
*********************************************/
ostream  out(ldp);

out << in->rdbuf() << flush;
ldp->close();



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
