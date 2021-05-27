#ifndef ROLL_OVER_H
#define ROLL_OVER_H

class RollOver {

private:
    /**************
    * CONSTANTS *
    ************/
    static const int PERIOD = 1<<14;
    static const int HALF_PERIOD = PERIOD>>1;


private:
    /*******
    * data *
    *******/
    int value;

public:
    /**************
    * constructor *
    **************/
    RollOver(int value);

    /*************
    * destructor *
    *************/
    virtual ~RollOver() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/

}; // end of RollOver class

#endif // ROLL_OVER_H
