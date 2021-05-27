#include "LDPStubRepository.h"

/*********************************************************************
* constructor
*********************************************************************/
LDPStubRepository::LDPStubRepository() {

    position = s.end();

} // end of constructor

/*********************************************************************
*
*********************************************************************/
int LDPStubRepository::getNextIndex(int product) {

    if(next_index.count(product)) {
        int index = next_index[product];
        next_index[product] = index+1;
        return index;
    }

    next_index[product] = 1;
    return 0;

} // end of getNextIndex method


/*********************************************************************
* returns true if we could determine the LDP that the packet belongs
* to - including if we are sure the packet is part of a new LDP
* sets the LDP in the packet.
* Otherwise the packet is not added to the repository and returns false.
*********************************************************************/
bool LDPStubRepository::addPacket(LDPPacketStub* stub) {

    /**************************************
    * slip the packet into the collection *
    **************************************/
    position = s.insert(position, stub);

    /****************************************
    * get the packets to the left and right *
    ****************************************/
    LDPPacketStub*  left = NULL;
    LDPPacketStub* right = NULL;
    
    if(position   != s.begin() )  {
        --position;
        left = *position;
        ++position;
    }

    ++position;
    if(position != s.end()   ) {
        right = *position;
    }
    --position;

    /*******************************
    * see if we belong on the right *
    *******************************/
    bool could_be_right = false;
    if(right != NULL) {
        try {
if(stub->getProduct()==84)cout << "    looking right"<<endl;
            if(stub->sameLDPAs(right)) {
                stub->setLDP(right->getLDP());
                return true;
            }
        } catch(LDPPacketStub::MaybeException& e) { could_be_right=true;}
    }
    
    /*******************************
    * see if we belong on the left *
    *******************************/
    bool could_be_left = false;
    if(left != NULL ) {
        try {
//if(stub->getProduct()==84)cout << "    looking left"<<endl;
            if(stub->sameLDPAs(left)) {
                stub->setLDP(left->getLDP());
                return true;
            }
        } catch(LDPPacketStub::MaybeException& e) {could_be_right=true;}
    }
    
//cout << "product="<<stub->getProduct()
    
    /*************************************************
    * if we get here, then there are no sure matches *
    *************************************************/
    if(!could_be_left && !could_be_right) {
        /**************************************************
        * we definitly don't belong to any existing LDPs
        * so we should start a new one of our own
        **************************************************/
        int product = stub->getProduct();
        int index = getNextIndex(product);
        
        if(index>0) cout << "Creating product "<<product<<" index "<<index<<endl;

        stub->setLDP(new LDPStub(product, index));
        return true;
    }
    
    /*************************************************************
    * if we know we don't belong on one side, that could tell us
    * something about belonging on the other side
    *************************************************************/
    

    
    /********************************************************************
    * if we get here, then we don't know for sure what LDP we belong to *
    ********************************************************************/
    s.erase(position);
    position =s.end();
    return false;
    


} // end of addPacket method

/*********************************************************************
* add an actual packet. returns the LDP that the packet belongs to or
* NULL if that couldn't be determined.
*********************************************************************/
LDPStub* LDPStubRepository::addPacket(LDPPacket* packet) {

#ifdef DEBUG
if(packet->product()==1)
cout << "adding apid="<<packet->apid()
     << " product="<<packet->product()
     << " page="<<packet->page()
     << " seq="<<packet->sequence()
     << " time="<<packet->time()
     <<endl;
#endif

    LDPPacketStub* stub = new LDPPacketStub(packet);
    if(addPacket(stub)) {
        /*********************
        * sucessfully added *
        ********************/
        return stub->getLDP();
    } else {
        /*****************
        * the add failed *
        *****************/
        delete stub;
        return NULL;
    }
} // end of addPacket method

/*********************************************************************
* add a packet with a known LDP index.
* the index must be unique, in otherwords only use this method for
* one packet from a particular LDP.
*********************************************************************/
void LDPStubRepository::initWith(LDPPacket* packet, int index) {

    /*************************
    * get the product number *
    *************************/
    int product = packet->product();
    
    /***************************************************
    * update the next available index for this product *
    ***************************************************/
    if(!next_index.count(product) || next_index[product] <= index) {
        next_index[product] = index+1;
    }

    /************************************************
    * create a stub for this packet and set the LDP *
    ************************************************/
    LDPPacketStub* stub = new LDPPacketStub(packet);
    stub->setLDP(new LDPStub(product, index));

    /***************************************
    * insert the packet into the container *
    ***************************************/
    position = s.insert(position, stub);


} // end of initWith method
