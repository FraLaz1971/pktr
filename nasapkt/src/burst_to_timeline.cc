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

ui->setDefault("timeline","burst_timeline");
ui->setDefault("modified","burst_timeline.tmp");
ui->setDefault("id",1);
ui->setDefault("burst_time",0.0);
ui->setDefault("pre",60.*10.);
ui->setDefault("after",60.*30.);

double start = burst_time - pre;
double stop  = burst_time + after;


{ // local block so destructors are called before exiting


/********************
* read the timeline *
********************/
TimeLine* timeline   = new TimeLine();
timeline->read(ui->getIstream("timeline") );

/******************************************
* check if it already contains this burst *
******************************************/
TimeLine* sub = timeline.findIntervals(start, stop);


TimeInterval interval = 



additional->read(ui->getIstream("infile") );

original->mergeWith(additional);
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
