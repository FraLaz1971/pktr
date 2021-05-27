//#include "Buff.h"
/****************************************************************************
*
* DESCRIPTION: This tool decodes one or more Swift large data products
* from a stream of packets and places the results in one or more files.
*
* PARAMETERS:
*
* - infile This is the file containing the LDP packets to be decoded.
*
* - outfile This is the name of the file where the decoded results will
*   be written. See the "split_output_files" parameter for more information.
*
* - checksum Do the packets in the input file contain checksums? Checksums
*   are optional according to the LDP specification, but both the XRT and
*   BAT currently plan to use them.
* 
* - format_check If set, the program will do a number of detailed format checks
*   on the incoming LDP packets. This option assumes that there is only one 
*   LDP (i.e. one APID and product number) in the incoming data 
*   [check this! - also add checksum verification to LDPChecker]
* 
* - split_output_files If set and the input data contain more than one LDP,
*   then each LDP will go to a different file. The name of each file will be
*   the file specified in the output parameter with the APID and product
*   number appended. Note this option is ignored if the output is being sent
*   to stdout (i.e. outfile="-").
*
* - header_strip If set, the program will strip off the standard LDP header
*   plus the number of bytes specified by the extra_header_size parameter
*   before writing the output data.
*   You might choose this option
*   if you are running decode_ldp on the output from encode_ldp. That programs
*   prepends the standard part of the LDP header to the original data, so this
*   option would restore the original file extra_header_bytes=0.
* 
* - extra_header_size
*   This is the size of the non-standard part of the LDP header to be stripped
*   from the output if the header_strip parameter is set.
*
* 
*
****************************************************************************/

#include "Writer.h"
#include "Reader.h"
#include "ChecksumLDPPacket.h"
#include "PacketPipe.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include <fstream>

#include "LDPBuffer.h"
#include "StandardLDPHead.h"
#include "LDPChecker.h"

#include "CommandLineUI.h"

#include <sstream>

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



/****************************************************************************
*
****************************************************************************/
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
ui->setDefault("infile","-");
ui->setDefault("outfile","-");
ui->setDefault("checksum", true);
ui->setDefault("format_check", false);
ui->setDefault("split_output_files", false);
ui->setDefault("header_strip", true);
ui->setDefault("extra_header_size", 0);


/************************
* read the command line *
************************/
bool has_checksum       = ui->getBoolean("checksum");
bool check_format       = ui->getBoolean("format_check");
bool split_output_files = ui->getBoolean("split_output_files");
bool strip_header       = ui->getBoolean("header_strip");
int extra_header_size   = ui->getInteger("extra_header_size");

/*************************************************************
* test to be sure the Reader is compatible with the hardware *
*************************************************************/
try {
    Reader::checkMachine();
} catch(Reader::HardwareException e) {
    cerr << "Hardware exception:" << e.what() << "\n";
    exit(1);
}




/*******************************
* initialize the packet reader *
*******************************/
LDPPacket* packet_prototype;
if(has_checksum) packet_prototype = new ChecksumLDPPacket();
else             packet_prototype = new LDPPacket();

istream* raw_in = ui->getIstream("infile");

PacketReader reader(raw_in, packet_prototype);



/*********************************************************
* initialize the LDP checker, though we might not use it *
*********************************************************/
LDPChecker checker;

/****************************
* initialize the LDP buffer *
****************************/
LDPBuffer* ldp = new LDPBuffer();

/******************************
* connect the pieces together *
******************************/
if(check_format) {
    /****************************
    * insert the format checker *
    ****************************/
    reader | checker |ldp->pipe();

} else {
    /*******************************
    * leave out the format checker *
    *******************************/
    reader | ldp->pipe();
}


/********************************************************************
* loop over all the LDPs - Error checking makes this complicated.
* The whole thing could be compressed to maybe 10 lines of code
* if we could be assured that the input data were perfect and that 
* the output stream was infaliable.
********************************************************************/
while(ldp->moreData() ) {

    /*************************************************************************
    * at the top of the loop we are at the beginning of a large data product *
    *************************************************************************/

    /*******************************
    * read the standard LDP header *
    *******************************/
    StandardLDPHead* head = new StandardLDPHead();
    try { 
        Reader* r = new Reader(new istream(ldp));
        head->read(r);
    } catch(Interpreter::Exception& e) {
        /************************************
        * couldn't read the standard header *
        ************************************/
        cerr << "Couldn't read standard header\n";

        /************************************************
        * skip to the next iteration if we are at the 
        * beginning of the next product
        ***********************************************/
        if(ldp->newProduct() ) continue;

        /***************************
        * otherwise report the gap *
        ***************************/
        int bytes = ldp->missingBytes();
        int pages = ldp->missingPages();

        cerr << "missing "<<pages<<" pages";

        if(bytes != LDPBuffer::unknown ) {
            cerr << " ("<<bytes<<" bytes)";
        }
            
        cerr << " in APID " << ldp->apid()
             << " product " << ldp->product()
             << "\n";

    
    }

    /******************************************************
    * determine what output stream we should send data to *
    ******************************************************/
    ostream* out;
    const string outfile = ui->getString("outfile");
    if(split_output_files && outfile != "-" ) {
        /***************************************************************
        * create a filename which contains the apid and product number *
        ***************************************************************/
        ostringstream tmp;
        tmp << outfile << ldp->apid() <<"_"<< ldp->product();
#       ifdef FAKING_STRINGSTREAM
	tmp << ends;
#       endif
        string filename = tmp.str();
        out = new ofstream(filename.c_str());
        cerr << "writing to "<<tmp.str()<<"\n";
        
    } else {
        out = ui->getOstream("outfile");
    }


    /*******************************************************
    * if we are not stripping the header, send the header 
    * to the output
    *******************************************************/
    if(!strip_header) {
        try {
            Writer w(out);
            head->write(&w);
        } catch(Interpreter::Exception& e) {
            /****************
            * error writing *
            ****************/
            cerr << "Error writing standard header for"
                 << " in APID " << ldp->apid()
                 << " product " << ldp->product()
                 << "\n";

            exit(1);
        }
    } 


    /*****************************************
    * read the non-standard header and throw 
    * it away, if that's what we want to do
    *****************************************/
    if(strip_header && extra_header_size>0 ) {

        /*****************************************************************
        * check if we encountered a gap when we read the standard header
        * and if so subtract it from the amount we will skip
        *****************************************************************/
        int skip;
        int missing_bytes = ldp->missingBytes();
        if(missing_bytes == 0 ) {
            /**********************************
            * no gap, so read the whole thing *
            **********************************/
            skip=extra_header_size;
        } else {
            /***********************************************
            * there was a gap so try to subtract it from
            * the bytes we will skip. If we can't tell how
            * big the gap was, then we won't skip any
            ***********************************************/
            if(missing_bytes != LDPBuffer::unknown) {
                skip = extra_header_size - missing_bytes;
            } else {
                skip = 0;
            }
        } // end if check for whether there was a gap
                
        if(skip>0) {
            /*************************
            * now we read the bytes *
            ************************/
            istream in(ldp);

            char buffer[extra_header_size];
            in.read(buffer,extra_header_size);

            /**********************************************
            * make sure we got all the bytes we requested *
            **********************************************/
            if((int)in.gcount() != extra_header_size) {
                /******************************
                * couldn't read all the bytes *
                ******************************/
                cerr << "Couldn't read the non-standard header\n";
                if(ldp->newProduct() ) continue;

            } // end if there was a problem reading the bytes

        } // end if we actually have some bytes to skip

    } // end if we should try to trim off the non-standard header

    /************************************************
    * try to dump all the data to the output stream *
    ************************************************/
    *out << ldp;

    /*************************************************************
    * if we came up short, keep dumping until we get to the end
    * of the current product
    *************************************************************/
    while( ! ldp->newProduct() ) {

        /*********************************************************
        * if it wasn't the end of a product that made us stop 
        * then it had to be a gap. Report the gap to stderr
        *********************************************************/
        int bytes = ldp->missingBytes();
        int pages = ldp->missingPages();

        cerr << "missing "<<pages<<" pages";

        if(bytes != LDPBuffer::unknown ) {
            cerr << " ("<<bytes<<" bytes)";
        }
            
        cerr << " in APID " << ldp->apid()
             << " product " << ldp->product()
             << "\n";

        /**************************
        * now read the next chunk *
        **************************/
        *out << ldp;

    } // end of loop over additional chunks in this product

    /*****************************
    * check for any write errors *
    *****************************/
    if(!out->good()) {
        cerr << "Error writing standard header for"
             << " in APID " << ldp->apid()
             << " product " << ldp->product()
             << "\n";
        exit(1);
    }
    

    /***********************************************
    * close the file by deleting the output stream *
    ***********************************************/
    if(out != &cout) delete out;

    /******************************************
    * see if the checker found anything wrong *
    ******************************************/
    if(checker.foundError()) {
        cerr << checker.error() << "\n";
    }

} // end of loop over products


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
