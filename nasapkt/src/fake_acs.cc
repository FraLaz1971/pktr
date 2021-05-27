/*****************************************************************************
*
* DESCRIPTION:
*
* PARAMETERS:
*
******************************************************************************/

#include "CommandLineUI.h"
#include "PacketWriter.h"
#include "TimeLine.h"
#include "ACS.h"


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
ui->setDefault("outfile","-");
ui->setDefault("interval", 1.0);
ui->setDefault("ra", 0.0);
ui->setDefault("dec", 0.0);
ui->setDefault("roll", 0.0);


/*************************************************************
* test to be sure the Writer is compatible with the hardware *
*************************************************************/
try {
    Writer::checkMachine();
} catch(Reader::HardwareException e) {
    cerr << "Hardware exception:" << e.what() << "\n";
    exit(1);
}

{ // local block so destructors are called before exiting

/********************
* read the timeline *
********************/
TimeLine timeline;
timeline.read(ui->getIstream("timeline"));

/***********************
* initialize the ACS packet *
****************************/
ACS* acs = new ACS();

acs->ra(  ui->getReal("ra"  ));
acs->dec( ui->getReal("dec" ));
acs->roll(ui->getReal("roll"));

acs->ten_arcmin(1);
acs->settled(1);

/***********************
* set up packet writer *
***********************/
PacketWriter writer(ui->getOstream("outfile"));

int nintervals = timeline.getIntervalCount();
for(int i=0; i<nintervals; ++i) {

    TimeInterval* interval = timeline.getInterval(i);

    double start = interval->start();
    double stop  = interval->stop();
    unsigned int id = interval->id();

    unsigned int obs = (id & 0xff);
    unsigned int target = (id >>8);
    
    acs->target(target);
    acs->obs(obs);
    
    int npackets = (int)((stop-start)/ui->getReal("interval"));
    double delta=(stop-start)/(npackets-1);
    for(int packet=0; packet< npackets; ++packet) {
        double time = start + packet*delta;
        acs->time()->value(time);
    }

    
    cout << start <<" "<< stop<<" "<<id<<endl;
    
}
    





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
