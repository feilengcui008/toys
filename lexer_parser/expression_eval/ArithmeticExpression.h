#ifndef ArithmeticExpression_H

#define ArithmeticExpression_H

#include <string>
#include "Expression.h"

using namespace std;


/*
 *
 *  abstract class for arthmetic expression 
 *
 */

class ArithmeticExpression : public Expression {
    public:
        ArithmeticExpression() = default;
        ArithmeticExpression(Expression *l, Expression *r);
        ~ArithmeticExpression();
        virtual string evaluate() = 0;
        virtual void print() = 0;
        float stof(string s);
        string ftos(float);
        Expression *left;
        Expression *right;
};

#endif
