#ifndef Subtraction_H

#define Subtraction_H

#include "ArithmeticExpression.h"

class Subtraction : public ArithmeticExpression {
    public:
        Subtraction() = default;
        ~Subtraction();
        Subtraction(Expression *l, Expression *r) : ArithmeticExpression(l, r){}
        string evaluate();
        void print(); 
};

#endif
