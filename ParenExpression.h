#ifndef PAREN_EXPRESSION_H
#define PAREN_EXPRESSION_H

#include "Expression.h"

/*****************************************************************************
* This is a special kind of Expression which can't actually be evaluated.
* It just acts as a placeholder in a TokenList to represent the grouping
* symbols "(" and ")". When collecting args a left paren looks for the
* pattern "(e)" where "e" is a single evaluable expression. Since a paren
* is never evaluable, this forces the things inside a pair of parents to be
* grouped into a single node of the expression tree.
*****************************************************************************/
class ParenExpression : public Expression {

private:
    /*******
    * data *
    *******/
    bool left;


public:
    /**************
    * constructor *
    **************/
    ParenExpression(bool left);

    /*************
    * destructor *
    *************/
    virtual ~ParenExpression() {}

    /************
    * accessors *
    ************/
    virtual bool isEvaluable() { return false; }

    /**********
    * methods *
    **********/
    static Expression* eat(istream* in);
    virtual double evaluate(Context* c) throw(Exception);
    virtual void collectArgs(TokenList* tokens);

    virtual void print(ostream& out) const;



}; // end of ParenExpression class

#endif // PAREN_EXPRESSION_H
