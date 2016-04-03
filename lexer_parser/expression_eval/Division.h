#ifndef Division_H

#define Division_H

#include "ArithmeticExpression.h"

class Division : public ArithmeticExpression {
    public:
        Division() = default;
        ~Division();
        Division(Expression *l, Expression *r) : ArithmeticExpression(l, r) {}
        string evaluate();
        void print(); 
};

#endif
