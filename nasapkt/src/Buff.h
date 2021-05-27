#include <iostream.h>
#include <streambuf.h>

using namespace std;

/***************************************************************************
* This is just test code which overrides all the methods in a streambuf
* in order to figure out how one works.
***************************************************************************/
class Buff : public streambuf {

private:
    int position;
    char* str;


public:
    Buff() {
        cout << "Buff constructor:\n"; 
        position=0;
//_M_mode = ios_base::out;
        str="abcd  efgh ijklm nopqrs";

char* buffer = (char*)malloc(10);
setg(str, str, str+10);

    }

    virtual int overflow(int c = EOF) {
        cout << "overflow called c="<<c<<"\n";
        return streambuf::overflow(c);
    }


    virtual int underflow() {
        cout << "underflow called position="<<position<<"\n";

char* b = gptr();
//cout << "gpptr="<<b<<"\n";
if(b==str+10) {
    setg(b,b, b+10);
    return (int)*b;
}
else  return EOF;

        return streambuf::underflow();

        if(position<strlen(str)) return str[position];
        else {
            return EOF;
        }
    }


    /****************************
    * this is the main one here *
    ****************************/
    virtual int uflow() {
        cout << "uflow called position="<<position<<"\n";
        return streambuf::uflow();

        if(position<strlen(str)) return str[position++];
        else {
            position=0;
            return EOF;
        }
    }


    virtual int pbackfail(int c) {
        cout << "pbackfail called c="<<(int)c<<"\n";
        --position;
        return 0;
        return streambuf::pbackfail(c);
    }

    virtual streamsize xsputn(const char* s, streamsize n) {
        cout << "xsputn called n="<<n<<"\n";
        return streambuf::xsputn(s,n);
    }

    virtual streamsize xsgetn(char* s, streamsize n) {
        cout << "xsgetn called n="<<n<<"\n";
        streamsize nread = streambuf::xsgetn(s,n);
        cout << "xsgetn read "<< nread<<"\n";
        return nread;

    }

/*
    virtual streampos seekoff(streamoff off, _seek_dir dir, int mode=ios::in|ios::out) {
        cout << "seekoff called\n";
        return streambuf::seekoff(off,dir,mode);
    }


    virtual streampos seekpos(streampos pos, int mode = ios::in|ios::out) {
        cout << "seekpos called\n";
        return streambuf::seekpos(pos,mode);
    }
*/

    virtual streambuf* setbuf(char* p, int len) {
        cout << "setbuf called\n";
        return streambuf::setbuf(p,len);
    }

    virtual int sync() {
        cout << "sync called\n";
        return streambuf::sync();
    }


/*
    virtual int doallocate() {
        cout << "doallocate called\n";
        return streambuf::doallocate();
    }
*/


    virtual int showmanyc() {
        cout << "showmanyc called\n";
        return streambuf::showmanyc();
    }
/*

    virtual void imbue(void * p) {
        cout << "imbue called\n";
        streambuf::imbue(p);
    }
*/


}; // end of Buff class
