#include <fitsio.h>
#include <longnam.h>

#include <fstream.h>
#include <vector.h>

#include "XRTLDPHead.h"
#include "XRTFrameHead.h"
#include "XRTPhotonCountingEvent.h"

main(int argc, char** argv) {

char* infile="mn19990100s101101m.unf";
char* outfile="xrt.ldp";

/*********************
* open the fits file *
*********************/
int status=0;
fitsfile* fp;
fits_open_file(&fp, infile, READONLY, &status);

/***************************
* determine the instrument *
***************************/
char instrument[FLEN_VALUE];
fits_read_key_str(fp, "INSTRUME", instrument, NULL, &status);
string sis0("SIS0");
string sis1("SIS1");

int inst;
if(     sis0 == instrument ) inst=0;
else if(sis1 == instrument ) inst=1;
else { 
    cerr << "Invalid instrument "<<instrument<<"\n";
    exit(1);
}

cout << instrument <<" "<<inst<<"\n";

/********************
* read the CCD mode *
********************/
char key[FLEN_KEYWORD];
sprintf(key,"S%dCCDMOD", inst);
long ccdmode;
fits_read_key_lng(fp, key, &ccdmode, NULL, &status);

if(ccdmode != 1) {
    cerr << "Invalid CCD mode "<<ccdmode<<"\n";
    exit(1);
}




/***************************
* determine the active CCD *
***************************/
char ccdlist[FLEN_VALUE];
sprintf(key,"S%dCCDLST",inst);
fits_read_key_str(fp, key, ccdlist, NULL, &status);

int chip;
sscanf(ccdlist,"%d",&chip);

cout << "chip="<<chip<<"\n";

/*******************************************
* read the lower level discriminator value *
*******************************************/
sprintf(key,"S%d_LVDL%d",inst,chip);
long lower_discrim;
fits_read_key_lng(fp, key, &lower_discrim, NULL, &status);

sprintf(key,"S%d_LVDU%d",inst,chip);
long upper_discrim;
fits_read_key_lng(fp, key, &upper_discrim, NULL, &status);



cout << "lower_discrim="<<lower_discrim
     << "upper_discrim="<<upper_discrim
     <<\n";


/*************************
* get the number of rows *
*************************/
fits_movnam_hdu(fp, ANY_HDU, "EVENTS", 0/* any version*/, &status);

long nrows; 
fits_read_key_lng(fp, "NAXIS2", &nrows, NULL, &status);

cout << "nrows="<<nrows<<"\n";


/*************************
* locate all the columns *
*************************/
int time_col;
int rawx_col;
int rawy_col;
int phas_col;

fits_get_colnum(fp, CASESEN, "TIME", &time_col, &status);
fits_get_colnum(fp, CASESEN, "RAWX", &rawx_col, &status);
fits_get_colnum(fp, CASESEN, "RAWY", &rawy_col, &status);
fits_get_colnum(fp, CASESEN, "PHAS", &phas_col, &status);


/************************************************
* open the output file and write the LDP header *
************************************************/
Writer* w = new Writer(new ofstream(outfile));

XRTLDPHead* ldp_head = new XRTLDPHead();
ldp_head->write(w);
delete ldp_head;

/******************************
* initialize the frame header *
******************************/
XRTFrameHead* frame_head = new XRTFrameHead();

frame_head->ccdFrameCnt(0);
frame_head->frameHeaderID(0x8073AB6F);
frame_head->lowerLevelThreshold(lower_discrim);
frame_head->upperLevelThreshold(upper_discrim);
frame_head->ccdExposure(4);
frame_head->splitThreshold(40);

/**********************************
* make a container to hold frames *
**********************************/
vector<XRTPhotonCountingEvent*>* events = new vector<XRTPhotonCountingEvent*>();

/***********************
* loop over CCD frames *
***********************/
double last_time=0.;
for(long row=1; row<=nrows; ++row) {

    /**********************
    * read the next event *
    **********************/
    int anynull=0;
    double time;
    int rawx;
    int rawy;
    int phas[9];
    fits_read_col_dbl(fp, time_col, row, 1l,1l, 0., &time,&anynull,&status);
    fits_read_col_int(fp, rawx_col, row, 1l,1l, 0 , &rawx,&anynull,&status);
    fits_read_col_int(fp, rawy_col, row, 1l,1l, 0 , &rawy,&anynull,&status);
    fits_read_col_int(fp, phas_col, row, 1l,9l, 0 , phas,&anynull, &status);

    /****************************************
    * copy the event to an XRT event object *
    ****************************************/
    XRTPhotonCountingEvent* event = new XRTPhotonCountingEvent();
    event->rawX(rawx);
    event->rawY(rawy);
    event->ph0(phas[0]);
    event->ph1(phas[1]);
    event->ph2(phas[2]);
    event->ph3(phas[3]);
    event->ph4(phas[4]);
    event->ph5(phas[5]);
    event->ph6(phas[6]);
    event->ph7(phas[7]);
    event->ph8(phas[8]);

    /***************************
    * save the event in a list *
    ***************************/
    events->insert(events->end(), event);

    /*******************************
    * increment the pixel counters *
    *******************************/
    for(int i=0; i<9; ++i) {

        if(phas[i] > lower_discrim ) {
            frame_head->greaterThanLLDCnt(frame_head->greaterThanLLDCnt()+1);
        }

        if(phas[i] > upper_discrim ) {
            frame_head->greaterThanULDCnt(frame_head->greaterThanULDCnt()+1);
        }
    }

    /***************************
    * determine the event type *
    ***************************/


    /******************************************
    * check if we are still in the same frame *
    ******************************************/
    if(time!=last_time) {
        /**************************************************
        * start of new frame, so write out the last frame *
        **************************************************/
        frame_head->frameStartTime()->value(last_time-2.0);
        frame_head->frameStopTime() ->value(last_time+2.0);
        
        frame_head->write(w);

        frame_head->ccdFrameCnt( frame_head->ccdFrameCnt() +1);
        cout << "new frame\n";
    }

    /******************
    * write the event *
    ******************/
 //   cout << row <<" "<<time<<"\n";

    /********************
    * remember the time *
    ********************/
    last_time=time;


} // end of loop over rows

cout << "status="<<status<<"\n";

fits_report_error(stderr,status);

} // end of main 
