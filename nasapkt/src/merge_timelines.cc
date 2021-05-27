/*****************************************************************************
*
* DESCRIPTION: 
*
* PARAMETERS:
*
******************************************************************************/

#include "CommandLineUI.h"
#include "Interpreter.h"
#include "TimeLine.h"


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

ui->setDefault("timeline","-");
ui->setDefault("infile","timeline");
ui->setDefault("outfile","-");
ui->setDefault("mode","add");



{ // local block so destructors are called before exiting

TimeLine* original   = new TimeLine();
TimeLine* additional = new TimeLine();


/**************************
* read the first timeline *
**************************/
try {
    original  ->read(ui->getIstream("timeline") );
} catch(TimeInterval::Exception& e) {
    cerr << "Error reading "<< ui->getString("timeline")<<endl;
    cerr << e.what() <<endl;
    exit(1);
}

/***************************
* read the second timeline *
***************************/
try {
    additional->read(ui->getIstream("infile") );
} catch(TimeInterval::Exception& e) {
    cerr << "Error reading "<< ui->getString("infile")<<endl;
    cerr << e.what() <<endl;
    exit(1);
}

/********
* merge *
********/
try {
    string mode = ui->getString("mode");
    
   // cout << "merge_timeline: mode="<<mode<<endl;

    if(     mode == "add"     ) original->mergeWith(additional);
    else if(mode == "force"   ) original->overlay(  additional);
    else if(mode == "subtract") original->subtract( additional);
    else if(mode == "inconsistencies") original->leaveInconsistencies(additional);
    else {
        cerr << "Unknown mode "+mode<<endl;
        exit(1);
    }

} catch(TimeInterval::Exception& e) {
    cerr << "Error merging timelines\n";
    cerr << e.what() <<"\n";
}

/********
* write *
********/
original->write(ui->getOstream("outfile"));

delete original;
delete additional;

} // end of local block so destructors are called before exiting
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
