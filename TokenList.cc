#include "Expression.h"

#include "FieldExpression.h"
#include "ConstantExpression.h"
#include "PlusMinusExpression.h"
#include "TimesOverExpression.h"
#include "ComparisonExpression.h"
#include "LogicExpression.h"
#include "NotExpression.h"
#include "ParenExpression.h"
#include "StringExpression.h"
#include "AssignmentOperator.h"
#include "VariableExpression.h"
#include "BitwiseExpression.h"

#include <sstream>



/*********************************************************************
* constructor
*********************************************************************/
Expression::TokenList::TokenList() {
} // end of constructor

/*********************************************************************
*
*********************************************************************/
void Expression::TokenList::append(Expression* e) {

    tokens.insert(tokens.end(), e);
}

/*************************************************************************
* reset the current token to the first in the list and return that token.
*************************************************************************/
Expression* Expression::TokenList::first() {

    current=tokens.begin();
    return *current;
}

/*************************************************************************
* advance the current token by one and return that token.
*************************************************************************/
Expression* Expression::TokenList::next() {

    if(current != tokens.end() ) ++current;
    return *current;
}

/*************************************************************************
* advance the current token by one and return that token.
*************************************************************************/
bool Expression::TokenList::moreTokens() {

    return tokens.size() > 0 &&  current != tokens.end();
}

/*************************************************************************
* returns the number of tokens in the list
*************************************************************************/
int Expression::TokenList::getCount() {

    return tokens.size();
}

/*************************************************************************
* returns the number of tokens in the list
*************************************************************************/
Expression* Expression::TokenList::get(int offset) {

    if(offset < tokens.begin() - current ) return NULL;
    if(offset > tokens.end() - current - 1) return NULL;

    return *(current+offset);
}

/*************************************************************************
* returns the number of tokens in the list
*************************************************************************/
void Expression::TokenList::remove(int offset) {

    if(offset!= 0) tokens.erase(current+offset);
    else {
        /***********************************************
        * removing current element - back up one first *
        ***********************************************/
        if(current==tokens.begin()) {
            /***************************************************
            * this is the first argument so go to end
            * to represent backing up before the first element 
            ***************************************************/
            tokens.erase(current);
            current = tokens.end();
        } else {
            --current;
            tokens.erase(current+1);
        }

    }

}





/*********************************************************************
*
*********************************************************************/
void Expression::TokenList::parse(const string& text) throw(Exception) {

/***************************
* clear out any old tokens *
***************************/
tokens.clear();

/***************************************
* parse a token list out of the string *
***************************************/
istream* in = new istringstream(text.c_str());

while(*in) {

    Expression* e=NULL;

    /********************
    * try reading a +/- *
    ********************/
    e = PlusMinusExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }

    /*****************************
    * multiplication or division *
    *****************************/
    e = TimesOverExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }

    /*****************************
    * multiplication or division *
    *****************************/
    e = BitwiseExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }


    /*************
    * comparison *
    *************/
    e = ComparisonExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }

    /*********
    * and/or *
    *********/
    e = LogicExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }

    /******
    * not *
    ******/
    e = NotExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }

    /********
    * paren *
    ********/
    e = ParenExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }


    /********
    * equals *
    ********/
    e = AssignmentOperator::eat(in);
    if(e) {
        append(e);
        continue;
    }


    /*********
    * string *
    *********/
    e = StringExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }


    /*************************
    * try reading a constant *
    *************************/
    e = ConstantExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }

    /*************************
    * try reading a variable *
    *************************/
    e = VariableExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }

    /****************************
    * try reading a telem field *
    ****************************/
    e = FieldExpression::eat(in);
    if(e) {
        append(e);
        continue;
    }


    /*******************************************
    * if we got here there was a parsing error *
    *******************************************/
    if(in->good() ) {
        throw Exception("Unknown token in expression");
    }

}

/***************************************
* make sure there were no other errors *
***************************************/
if(! in->eof() ) {
    throw Exception("I/O error while parsing expression");
}


/**********************************
* initialize the current position *
**********************************/
first();

} // end of parse method


/*********************************************************************
*
*********************************************************************/
void Expression::TokenList::collect() throw(Exception) {

//cout << "collecting "<<this<<"\n";

/************************************
* make a first pass through the list *
*************************************/
for(first(); moreTokens(); next() ) {
    get(0)->firstPass(this);
}

/*****************************
* now collect all the tokens *
*****************************/
int old_ntokens=0;
while(getCount() > 1 && old_ntokens != getCount() ) {

    old_ntokens = getCount();

    for(first(); moreTokens(); next() ) {
        get(0)->collectArgs(this);

//cout << "after collection: "<< this<<"\n";

    }
}

/*********************************
* check if we got stuck anywhere *
*********************************/
if(getCount() != 1 ) {
    throw Exception("Expression syntax error");
}


} // end of collect method


/***************************************************************************
* printing operator
***************************************************************************/
ostream& operator<<(ostream& out, Expression::TokenList* e) {

    if(e==NULL) out << "null";
    else {
        for(vector<Expression*>::iterator it = e->tokens.begin();
            it != e->tokens.end(); ++it) {

            out << *it;
            if(it+1 != e->tokens.end() ) out << ", ";
        }
    }

    return out;

} // end of printing operator
