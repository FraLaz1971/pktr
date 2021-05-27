/*****************************************************************************
*
* DESCRIPTION: This tool sorts a stream of CCSDS packets. The stream is
* assumed to only contain one APID, if not, unpredictable reasults will 
* occur. Packets are sorted in their "natural order" - i.e. in the manner
* appropriate to their type as determined by a SwiftTypedPacketReader.
* - LDP packets are sorted by page number.
* - All other Swift packets are sorted by secondary header time and sequence 
*   number.
* - Generic CCSDS packets are sorted by sequence counter, though this is 
*   of limited usefulness since that counter is cyclical.
*
* The sort is stable, meaning that the original order is preserved when
* two packets are "the same".
*
* This tool can optionally "unique" the packet stream -i.e. delete
* all but the last of a string of identical packets. Note that this is 
* different from common "uniquing" practice, because we consider the last 
* packet in the input stream to be more valid. This way an earlier version
* of a packet will be replaced by a later revised version.
*
* PARAMETERS:
*
* - infile The input file name. 
* - outfile the output file name. The output file should not be the
*   same as the input file.
* - apid_list The name of a configuration file for the SwiftTypedPacketReader
*   which lists the packet types for various APIDs.
* - unique Should the packets be "uniqued" after sorting?
* - trash The name of a file to which packets deleted by uniquing will be
*   appended. If this is an empty string, deleted packets will be discarded.
*   This parameter is ignored if the "unique" parameter is false.
*
******************************************************************************/

#include "CommandLineUI.h"
#include "SwiftTypedPacketReader.h"
#include "PacketTee.h"
#include "PacketTank.h"
#include "PacketWriter.h"

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

ui->setDefault("infile", "-");
ui->setDefault("outfile", "-");
ui->setDefault("apid_list", "apids.list");
ui->setDefault("unique", true);
ui->setDefault("trash", "");


/*********************************
* create the typed packet reader *
*********************************/
const string& config= ui->getString("apid_list");
SwiftTypedPacketReader* reader = new SwiftTypedPacketReader(config, 
                                                      ui->getIstream("infile"));


/**************************************
* tee all the reader outputs together *
**************************************/
PacketTee tee;

reader->head2()   | tee;
reader->head3()   | tee;
reader->uvot()    | tee;
reader->ldp()     | tee;
reader->acs()     | tee;
reader->tdrss()   | tee;
reader->generic() | tee;


/***************************************************
* now connect up a tank and a writer 
* we use a straight section of pipe after the tank
* for pumping the tank dry after sorting
***************************************************/
{ // local block to ensure the following things get deleted before exit
PacketTank tank;
PacketPipe pipe;
PacketWriter writer(ui->getOstream("outfile"));

tee | tank | pipe | writer;


/****************************************************************
* fill the tank, sort the packets there and then flush the tank *
****************************************************************/
reader->pump();
tank.sort();
if(ui->getBoolean("unique") ) {
    /********************
    * unique the packets *
    *********************/
    PacketWriter* trash = NULL;
    if(ui->getString("trash") != "" ) {
        /*****************************************************
        * set up a trash file to receive any deleted packets *
        *****************************************************/
         trash = new PacketWriter(ui->getOstream("trash",ios::app) );
    }

    /***************************
    * actually do the uniquing *
    ***************************/
    tank.keepLastUnique(trash);
}
pipe.pump();

} // end of local block to make sure writer gets deleted

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
