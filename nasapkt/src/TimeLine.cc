#include "TimeLine.h"

#include <sstream>
#include <vector>
#include <iomanip>


/*****************************************************************************
* Add the interval to the timeline.
* This may merge the interval with another if they overlap.
* Throws an exception if two intervals with different IDs overlap.
*****************************************************************************/
void TimeLine::addInterval(TimeInterval* interval)
                   throw(TimeInterval::Exception) {
                   

/******************************
* add the interval to the set *
******************************/
add_position = s.insert(add_position, interval);


vector<TimeInterval*> deleted;

try {
    /************************************************
    * check if this position overlaps previous ones *
    ************************************************/
    if(add_position != s.begin() ) {

        multiset<TimeInterval*, lessThan>::iterator adjacent = add_position;
        --adjacent;

        while(add_position != s.begin() && 
              (*adjacent)->canMergeWith(*add_position) ) {

            /********************************************
            * merge this interval with the previous one *
            ********************************************/
            (*add_position)->mergeWith(*adjacent);
            deleted.insert(deleted.end(), *adjacent);
            s.erase(adjacent);


            /*************************************************
            * update the adjacent position
            * note that it was invalidated when we deleted
            * the element it pointed to 
            *************************************************/
            adjacent = add_position;
            --adjacent;        

        }
    }

    /****************************************************
    * now check the intervals which come after this one *
    ****************************************************/
    multiset<TimeInterval*, lessThan>::iterator adjacent = add_position;
    ++adjacent;

    while(adjacent != s.end() && 
          *adjacent != NULL && (*adjacent)->canMergeWith(*add_position) ) {

            /*********************************************
            * merge this interval with the following one *
            *********************************************/
            (*add_position)->mergeWith(*adjacent);
            deleted.insert(deleted.end(), *adjacent);
            s.erase(adjacent);

            /*************************************************
            * update the adjacent position
            * note that it was invalidated when we deleted
            * the element it pointed to 
            *************************************************/
            adjacent = add_position;
            ++adjacent;        


    } // end of loop over following intervals

    /**********************************************
    * we go through that safely, so now we can
    * destroy all the TimeInterval objectes that 
    * we removed from the list
    **********************************************/
    for(vector<TimeInterval*>::iterator it=deleted.begin(); 
        it!= deleted.end(); ++it) {

        delete (*it);

    }

} catch(TimeInterval::Exception& e) {
    /***************************************************
    * one of the merge attempts above was illegal
    * We need to reset the timeline to its initial state.
    * Luckily we saved all the intervals that we removed 
    * from the list.
    ***************************************************/
    s.erase(add_position);
    add_position=s.begin();

    for(vector<TimeInterval*>::iterator it=deleted.begin(); 
        it!= deleted.end(); ++it) {

        add_position = s.insert(add_position, *it);

    }

    /***********************************************
    * now throw the exception up to the next level *
    ***********************************************/
    throw e;

} // end of catch phrase


/*
cout << "timeline at end:"<<endl;
write(&cout);
cout << endl;
*/

} // end of addInterval method

/*****************************************************************************
* Delete the interval from the timeline. Throws an exception if some part of
* this interval is not in the timeline
*****************************************************************************/
void TimeLine::removeInterval(TimeInterval* interval)
               throw(TimeInterval::Exception) {

TimeLine* overlaps = findIntervals(interval->start(), interval->stop());

if(overlaps->getIntervalCount() == 1) {

    TimeInterval* overlap = getInterval(0);

    /*******************************************
    * make sure the intervals have the same ID *
    *******************************************/
    if(overlap->id() != interval->id() ) {
        throw TimeInterval::Exception("Removed interval "+
                                      interval->toString()+
                                      " has an ID inconsistent with timeline");
    }

    /***************************************************************
    * make sure the removed interval is entirely contained in an
    * interval in this timeline
    **************************************************************/
    if(overlap->start() > interval->start() ||
       overlap->stop()  < interval->stop()     ) {
       
       throw TimeInterval::Exception("Removed interval "+
                                      interval->toString()+
                                      " not entirely contained in timeline");
       
    }

    if(overlap->start() == interval->start() &&
       overlap->stop()  == interval->stop()     ) {
       /*********************************************
       * identical intervals, so delete the old one *
       *********************************************/
       s.erase(s.find(overlap));
       delete overlap;
       
    } else if(overlap->start() < interval->start() &&
              overlap->stop()  > interval->stop()     ) {
        /******************************************************
        * we removed a chunk from the middle of this interval
        * so we need to split the interval in two
        ******************************************************/

        TimeInterval* end = new TimeInterval(interval->stop(), overlap->stop(),
                                             overlap->id() );
        overlap->stop(interval->start());
        addInterval(end);

    } else if(overlap->start() == interval->start() ) {
        /*********************************
        * need to trim off the beginning *
        *********************************/
        overlap->start(interval->stop());

    } else if(overlap->stop() == interval->stop() ) {
        /******************************
        * need to trim off the end *
        ***************************/
        overlap->stop(interval->start());
    } else {
        cerr << "Impossible condition in removeInterval"<<endl;
        cerr << "This should not have occured"<<endl;
        exit(1);
    }

} else {
    /********************************************************************
    * one way or another the removed interval must cover a gap in this
    * timeline
    ********************************************************************/
    cout << "overlaps count="<<overlaps->getIntervalCount()<<endl;
    overlaps->write(&cout);

    throw TimeInterval::Exception("No one interval in timeline corresponds to removed interval "+
                                  interval->toString());
}

} // end of removeInterval method

/*****************************************************************************
* delete all parts of this timeline that fall within the given interval
* and have the same ID
*****************************************************************************/
void TimeLine::removeIntervalWhereConsistent(TimeInterval* interval)
               throw(TimeInterval::Exception) {

TimeLine* overlaps = findIntervals(interval->start(), interval->stop());

/**********************************
* loop over overlapping intervals *
**********************************/
for(multiset<TimeInterval*, lessThan>::iterator it = overlaps->s.begin();
    it != overlaps->s.end(); ++it) {

    TimeInterval* overlap = *(it);

  //  cout << "    overlap: "<<overlap->start()<<" "<<overlap->stop()<<endl;


    /********************************
    * check if the IDs are the same *
    ********************************/
    if(overlap->id() == interval->id() ) {
        /*******************************************
        * same ID, so we need to remove
        * the overlapping parts. First we take out the entire
        * interval, and then we add back any parts which
        * don't overlap
        *******************************************/

        s.erase(s.find(overlap));
        if(   *add_position == overlap)    add_position=s.end();
        if(*search_position == overlap) search_position=s.end();





        if(overlap->start() <  interval->start() ) {
            /**************************************************
            * put back the part which comes before the overlap *
            **************************************************/
            addInterval(new TimeInterval( overlap->start(),
                                         interval->start(),
                                          overlap->id()    ));
        }

        if(overlap->stop() >  interval->stop() ) {
            /**************************************************
            * put back the part which comes after the overlap *
            **************************************************/
            addInterval(new TimeInterval(interval->stop(),
                                          overlap->stop(),
                                          overlap->id()   ));
        }


        delete overlap;


    } // end if intervals have the same ID

} // end of loop over overlaps

} // end of removeIntervalWhereConsistent method

/*****************************************************************************
* Add an interval to the timeline, silently obliterating any intervals
* or portions of intervals which overlap the new one.
*****************************************************************************/
void TimeLine::forceInterval(TimeInterval* interval) throw(TimeInterval::Exception) {

//cout << "forcing "<<setprecision(14) << interval->start() <<" - "<<interval->stop()<<endl;

TimeLine* overlaps = findIntervals(interval->start(), interval->stop());

for(int i=0; i< overlaps->getIntervalCount(); ++i) {

    TimeInterval* overlap =  overlaps->getInterval(i);

//cout << "    overlap: "<<setprecision(14) << overlap->start() <<" - "<<overlap->stop()<<endl;

    if(overlap->start() >= interval->start() &&
       overlap->stop() <= interval->stop()      ) {
        /*************************************************
        * the overlapping interval is contaned entirely
        * in the new interval, so just delete the
        * overlap
        ************************************************/

        s.erase(s.find(overlap));
    } else {
        /******************************************************
        * The interval hangs off the end, so we need to trim
        * it to save the non-overlapping pieces
        ******************************************************/
        if(overlap->start() <  interval->start() ) {
            /*****************************
            * need to trim the beginning *
            *****************************/
//cout << "        trimming start"<<endl;
            double original_stop = overlap->stop();
            overlap->stop(interval->start());

            if(original_stop > interval->stop() ) {
//cout << "        splitting"<<endl;
                /***********************************
                * need to add an interval to cover what comes over the
                * new interval
                *********************************************************/
                addInterval(new TimeInterval(interval->stop(), original_stop, overlap->id()) );
            }

            overlap->stop(interval->start());

        }

        if(overlap->stop() >  interval->stop() ) {
            /***********************
            * need to trim the end *
            ***********************/
//cout << "        trimming end"<<endl;
            overlap->start(interval->stop());

        }
    } // end if we need to trim

} // end of loop over overlapping intervals

/***************************************
* we're done with the overlap timeline *
***************************************/
delete overlaps;

/********************************************
* now we can just add the interval, being
* confident there are no remaining overlaps
********************************************/
addInterval(interval);

} // end of forceInterval method

/**************************************************************************
* Add a time interval, up to the point where it conflicts with this
* timeline. That means before adding the interval, it will be truncated
* to the start time of the first interval that it overlaps which has a
* different ID.
**************************************************************************/
// void TimeLine::suggestInterval(TimeInterval* interval) {
//
//
//
//
// } // end of suggestInterval method

/****************************************************************************
* find the interval in the timeline which includes a given time.
* If the time is not included in any interval, then return NULL.
****************************************************************************/
TimeInterval* TimeLine::findInterval(double time) {

//cout << "findInterval start\n";

/**************************************************
* don't bother searching if the timeline is empty *
**************************************************/
if(s.empty() ) return NULL;

if(search_position==s.end() ) -- search_position;

/*******************
* search backwards *
*******************/
while(search_position != s.begin() && (*search_position)->start() > time ) {
    --search_position;
}

//cout << "done searching backwards\n";

/*****************
* search forward *
*****************/
while(search_position != s.end() && (*search_position)->stop() < time ) {
    ++search_position;
}


//cout << "done searching\n";


/**********************************************************************
* by now we should have found it. If so return the result
**********************************************************************/
if(search_position == s.end() ) return NULL;

if( (*search_position)->contains(time) ) return *search_position;
else                                     return NULL;


} // end of findInterval method

/*****************************************************************************
* return the interval at a particular index
*****************************************************************************/
TimeInterval* TimeLine::getInterval(int index) {

    multiset<TimeInterval*, lessThan>::iterator it = s.begin();
    for(int i=0; i<index; ++i) ++it;
    
    return *it;

}

/*****************************************************************************
* return a new timeline which contains all time intervals which have any part
* within the given range of times. The caller has the responsibility of
* disposing of the new timeline
*****************************************************************************/
TimeLine* TimeLine::findIntervals(double start, double stop)  {

    TimeLine* timeline = new TimeLine();

    /**************************************************
    * don't bother searching if the timeline is empty *
    **************************************************/
    if(s.empty() ) return timeline;

    if(search_position==s.end() ) -- search_position;

    /**************************************
    * search backwards for the start time *
    **************************************/
    while(search_position != s.begin() && (*search_position)->stop() > start ) {
        --search_position;
    }

    /*********************************************************
    * search forward for the first interval within the range *
    *********************************************************/
    while(search_position != s.end() && (*search_position)->stop() < start ) {
        ++search_position;
    }


    /**************************************************************
    * now load everything in the time range into the new timeline *
    **************************************************************/
    while(search_position != s.end() && (*search_position)->start() < stop ) {

        /*************************************************************
        * add the interval. We don't ever expect an exception,
        * since we are just copying intervals from a valid timeline
        *************************************************************/
        try { timeline->addInterval(*(search_position++)); }
        catch(TimeInterval::Exception& e) {
            cerr << "Error in Timeline::findIntervals"<<endl;
            cerr << e.what() <<endl;
            cerr << "This should never occur"<<endl;
            exit(1);
        }

    } // end of loop over intervals


    return timeline;


} // end of findIntervals method


/*****************************************************************************
* fill in any gaps in the timeline. A gap is a span of time no greater that
* allowed_gap between adjacent intervals with the same ID
*****************************************************************************/
void TimeLine::fillGaps(double allowed_gap) {

//cout << "filling gaps"<<endl;

/****************************************
* don't bother if the timeline is empty *
****************************************/
if(s.size() <= 1) return;


try { // to catch impossible errors filling gaps

    vector<TimeInterval*> gaps;

    /*********************************************
    * loop over adjacent pairs of time intervals *
    *********************************************/
    multiset<TimeInterval*, lessThan>::iterator it = s.begin();
    TimeInterval* previous = *it;
    ++it;

    for(; it != s.end(); ++it) {
    
        TimeInterval* current = *it;

        /********************************
        * check if we can fill this gap *
        ********************************/
        double gap = current->start() - previous->stop();
        if(previous->id() == current->id() && gap<=allowed_gap && gap>0. ) {
            /**********************************************
            * yes we can, so save the interval which fills
            * the gap so we can add it later
            **********************************************/
            gaps.insert(gaps.end(), new TimeInterval(previous->stop(),
                                                     current->start(),
                                                     current->id()     ) );
        } // end if we can fill the gap

        /*****************************
        * save the previous interval *
        *****************************/
        previous=current;


     } // end of loop over pairs of intervals

    /************************************************************
    * now go back and insert all the gaps we collected
    * we do this instead of adding them ad we go to avoid any
    * confusion that might result from modifying the set
    * as we iterate over it.
    ***********************************************************/
    for(vector<TimeInterval*>::iterator it2=gaps.begin();
        it2 != gaps.end(); ++it2) {

        addInterval(*it2);
    }

} catch(TimeInterval::Exception& e) {
    /***************************
    * this should never happen *
    ***************************/
    cerr << "Impossible error in TimeLine::fillGaps" << endl;
    cerr << e.what() << endl;
    exit(1);
}



} // end of fillGaps method

/*****************************************************************************
* Fill in gaps between intervals, by splitting the difference
* between them. Unlike the fillGaps method, this will work for
* adjacent intervals with different IDs
*****************************************************************************/
void TimeLine::splitGaps(double allowed_gap, double latest_allowed_time) {

//cout << "filling gaps"<<endl;

/****************************************
* don't bother if the timeline is empty *
****************************************/
if(s.size() <= 1) return;


try { // to catch impossible errors filling gaps

    vector<TimeInterval*> gaps;

    /*********************************************
    * loop over adjacent pairs of time intervals *
    *********************************************/
    multiset<TimeInterval*, lessThan>::iterator it = s.begin();
    TimeInterval* previous = *it;
    ++it;

    for(; it != s.end(); ++it) {
    
        TimeInterval* current = *it;
        
        /***************************************
        * check if we are up to the time limit *
        ***************************************/
        if(current->stop() > latest_allowed_time) break;

        /********************************
        * check if we can fill this gap *
        ********************************/
        double gap = current->start() - previous->stop();
        if(gap<=allowed_gap && gap>0. ) {
            /**********************************************
            * yes we can, so save the intervals which fill
            * the gap so we can add them later
            * It gets wierd if we modify a container while 
            * we are iterating over it
            **********************************************/
            double midpoint = 0.5*(previous->stop() + current->start());
            
            gaps.insert(gaps.end(), new TimeInterval(previous->stop(),
                                                     midpoint,
                                                     previous->id()     ) );

            gaps.insert(gaps.end(), new TimeInterval(midpoint,
                                                     current->start(),
                                                     current->id()     ) );
        } // end if we can fill the gap

        /*****************************
        * save the previous interval *
        *****************************/
        previous=current;


     } // end of loop over pairs of intervals

    /************************************************************
    * now go back and insert all the gaps we collected
    * we do this instead of adding them ad we go to avoid any
    * confusion that might result from modifying the set
    * as we iterate over it.
    ***********************************************************/
    for(vector<TimeInterval*>::iterator it2=gaps.begin();
        it2 != gaps.end(); ++it2) {

        addInterval(*it2);
    }

} catch(TimeInterval::Exception& e) {
    /***************************
    * this should never happen *
    ***************************/
    cerr << "Impossible error in TimeLine::fillGaps" << endl;
    cerr << e.what() << endl;
    exit(1);
}



} // end of fillGaps method
/*****************************************************************************
* add all the intervals in the given timeline to this one
*****************************************************************************/
void TimeLine::mergeWith(TimeLine* timeline) throw(TimeInterval::Exception) {


    for(multiset<TimeInterval*, lessThan>::iterator it = timeline->s.begin();
        it != timeline->s.end(); ++it) {

        TimeInterval* interval = *(it);

        try { addInterval(interval); }
        catch(TimeInterval::Exception& e) {
            cerr << "Error merging timelines "
                 << e.what()
                 <<"\n";
        }


    } // end of loop over intervals


} // end of mergeWith method


/*****************************************************************************
* remove all the intervals in the given timeline to this one
*****************************************************************************/
void TimeLine::subtract(TimeLine* timeline) throw(TimeInterval::Exception) {


    for(multiset<TimeInterval*, lessThan>::iterator it = timeline->s.begin();
        it != timeline->s.end(); ++it) {

        TimeInterval* interval = *(it);

        removeInterval(interval);

    } // end of loop over intervals


} // end of subtract method

/*****************************************************************************
* calls removeIntervalWhereConsistent for all intervals in the given timeline
*****************************************************************************/
void TimeLine::leaveInconsistencies(TimeLine* timeline) throw(TimeInterval::Exception) {

    for(multiset<TimeInterval*, lessThan>::iterator it = timeline->s.begin();
        it != timeline->s.end(); ++it) {

        TimeInterval* interval = *(it);
        removeIntervalWhereConsistent(interval);



    } // end of loop over intervals


} // end of subtract method

/******************************************************************************
* force all the intervals in the given timeline into this one.
* In other words where the two timelines differ the given one dominates
* over this one.
******************************************************************************/
void TimeLine::overlay(TimeLine* timeline) throw(TimeInterval::Exception) {

    for(multiset<TimeInterval*, lessThan>::iterator it = timeline->s.begin();
        it != timeline->s.end(); ++it) {


        TimeInterval* interval = *(it);
        forceInterval(interval);

    } // end of loop over intervals

} // end of overlay method



/*****************************************************************************
* read the timeline in ASCII format from a file
*****************************************************************************/
void TimeLine::read(istream* in) throw(TimeInterval::Exception){

int length=1024;
char line[length];

/************************************
* loop over lines in the input file *
************************************/
while(in->getline(line,length).good() ) {

    /*********************
    * now parse the line *
    *********************/
    istringstream parse(line);
    double start;
    double stop;

    parse >> start >> stop;
    if(! parse.good() ) throw TimeInterval::Exception("file format error");

    int id=0;
    parse >> id;

    addInterval(new TimeInterval(start,stop,id) );

} // end of loop over lines
} // end of read method


/*****************************************************************************
* write the timeline in ASCII format to a file
*****************************************************************************/
void TimeLine::write(ostream* out) {

    *out << setprecision(15);

//cout << "writing timeline to "<<out<<endl;

for(multiset<TimeInterval*, lessThan>::iterator it = s.begin();
    it != s.end();
    ++it) {

   // cout << "    writing interval"<<endl;

    TimeInterval* interval = *it;


    *out << interval->start() << " "
         << interval->stop()  << " "
         << interval->id()    << endl;

} // end of loop over intervals

out->flush();

} // end of write method
