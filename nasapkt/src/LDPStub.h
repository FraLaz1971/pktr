#ifndef LDPSTUB_H
#define LDPSTUB_H

class LDPStub {

private:
    /*******
    * data *
    *******/

    int npages;
    int product;
    int index;


public:
    /**************
    * constructor *
    **************/
    LDPStub(int product, int index);

    /*************
    * destructor *
    *************/
    virtual ~LDPStub() {}

    /************
    * accessors *
    ************/
    virtual void setPageCount(int npages);
    virtual int  getPageCount();
    
    virtual int getProduct();
    virtual int getIndex();

    /**********
    * methods *
    **********/

}; // end of LDPStub class

#endif // LDPSTUB_H
