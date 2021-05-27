#include "DataBuf.h"

/*********************************************************************
* constructor
*********************************************************************/
DataBuf::DataBuf(char* buffer, int size) {

setg(buffer, buffer, buffer+size);
setp(buffer, buffer+size);


} // end of constructor

/*********************************************************************
*
*********************************************************************/

#ifdef OMIT

#include <iostream>
main(int argc, char** argv) {


int size=10;
char* data = new char[size];
for(int i=0; i<size; ++i)  data[i] = 0;

DataBuf* buf = new DataBuf(data, size);

std::ostream out (buf);

for(int i=0; i<10; ++i) {
    char byte =i;
std::cout <<"writing byte "<<(int)byte<<"\n";
    out.write(&byte,1);
}

for(int i=0; i<size; ++i) std::cout << "data["<<i<<"]="<<(int)data[i]<<"\n";


std::istream in(buf);


for(int i=0; in; ++i) {
    char byte;
    in.read(&byte,1);

    if(! in) break;

    std::cout << (int)byte<<"\n";
    

}

} // end of main

#endif // OMIT
