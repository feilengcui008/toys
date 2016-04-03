#ifndef Multiplication_H

#define Multiplication_H

#include "ArithmeticExpression.h"

class Multiplication : public ArithmeticExpression {
    public:
        Multiplication() = default;
        ~Multiplication();
        Multiplication(Expression *l, Expression *r) : ArithmeticExpression(l, r){}
        string evaluate();
        void print(); 
};


#endif
