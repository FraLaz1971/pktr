#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Context.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

/*****************************************************************************
* This class represents a parsed expression, which may be evaluated in the
* context of a packet. An expression is represented as a tree. This class is a
* single node of the tree. Subclasses represent various operations and the
* children of each node represent their arguments. Some expression, such as
* constants have no arguments.
*
* Parsing happens in three stages. First the input string is converted to a 
* TokenList of Expressions. Each expression subclass must know how to "eat"
* its text representation from an istream.
* At this stage The arguments to each Expression have not yet been set.
* Next, we make a "fist pass" through the TokenList. Currently the only thing
* we do in this pass is determine whether +/- operators are unary or binary.
* Finally, we construct the expression tree in multiple passes over the
* TokenList. In each pass each operator is asked to collect its arguments.
* An operator can only collect arguments if no other operator with higher
* precedence will want to use that argument. When an argument is collected
* it is removed from the TokenList. This continues until there is only one 
* Expression left in the list - the root node.
*****************************************************************************/
class Expression {

public:

/******************************************************************************
* This is an Exception class for anything which might go wrong when parsing
* or evaluating an expression
******************************************************************************/
class Exception : public runtime_error {

    public: Exception(const string& context) : runtime_error(context) { }

}; // end of Exception inner class


/******************************************************************************
* This is an inner class used for parsing an expression. It holds a list
* of Expressions which may not yet have had arguments assigned to them.
* It is an inner class so that it can contain an Expression but that 
* Expression can have methods with a TokenList as an argument
* The method bodies for this class are defined in TokenList.cc
******************************************************************************/
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
    virtual void remove(int offset);

    virtual Expression* get(int offset);


    virtual void parse(const string& text) throw(Exception);
    virtual void collect() throw(Exception);

    /************
    * operators *
    ************/
    friend ostream& operator<<(ostream& out, TokenList* e);


}; // end of TokenList class

        

protected:
    /*******
    * data *
    *******/
    vector<Expression*> args;


public:
    /**************
    * constructor *
    **************/
    Expression();

    /*************
    * destructor *
    *************/
    virtual ~Expression() {}

    /************
    * accessors *
    ************/
    virtual double arg(int i, Context* c);
 //   vitrual const string& toString() =0;

    /**********
    * methods *
    **********/
    virtual double evaluate(Context* c) throw(Exception) =0;
    virtual void set(Context* c, double value) throw(Exception);

    friend ostream& operator<<(ostream& out, Expression* e);
    virtual void print(ostream& out) const;

    virtual void firstPass(TokenList* tokens) {}
    virtual void collectArgs(TokenList* tokens) {}

    virtual bool isEvaluable();
    virtual bool hasArgs() { return false; }
    virtual bool hasLeftArg() { return false; }
    virtual bool hasRightArg() { return false; }

    virtual int precedence() { return 0; }

    /*****************
    * static methods *
    *****************/
    static Expression* parse(const string& text) throw (Exception);
    static Expression* eat(istream* in);



protected:
    static bool isWhiteSpace(char c) {
        return c==' ' || c=='\t' || c=='\n' || c=='\r';
    }

    static bool isWordChar(char c) {

        return (c>= 'a' && c <= 'z') || 
               (c>= 'A' && c <= 'Z') ||
               (c>= '0' && c <= '9') ||
               c == '_';

        return c==' ' || c=='\t' || c=='\n' || c=='\r';
    }


    static void skipWhiteSpace(istream* in) {
        while(isWhiteSpace(in->peek()) ) in->get();
    }

}; // end of Expression class

#endif // EXPRESSION_H
