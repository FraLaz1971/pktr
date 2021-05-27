#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include "Expression.h"
#include <vector>
#include <string>

using namespace std;

class TokenList {

private:
    /*******
    * data *
    *******/
    vector<Expression*> tokens;
    vector<Expression*>::iterator current;


public:
    /**************
    * constructor *
    **************/
    TokenList();

    /*************
    * destructor *
    *************/
    virtual ~TokenList() {}

    /************
    * accessors *
    ************/

    /**********
    * methods *
    **********/
    virtual void append(Expression* e);
    virtual Expression* first();
    virtual Expression* next();
    virtual bool moreTokens();
    virtual int getCount();

    virtual Expression* token(int offset);


    virtual void parse(const string& text);
    virtual void collect();

    /************
    * operators *
    ************/
    friend ostream& operator<<(ostream& out, TokenList* e);


}; // end of TokenList class

#endif // TOKEN_LIST_H
