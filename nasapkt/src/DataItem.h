#ifndef DATA_ITEM_H
#define DATA_ITEM_H

#include "DataBlock.h"
#include <string>

class DataItem : public DataBlock {

protected:
    /*******
    * data *
    *******/
    string name;
    string description;


public:
    /**************
    * constructor *
    **************/
    DataItem();

    /*************
    * destructor *
    *************/
    virtual ~DataItem() {}

    /************
    * accessors *
    ************/
    public void setName(string name);
    public void setDescription(string description);

    /**********
    * methods *
    **********/

}; // end of DataItem class

#endif // DATA_ITEM_H
