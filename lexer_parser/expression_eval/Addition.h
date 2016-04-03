#ifndef Addition_H

#define Addition_H

#include "ArithmeticExpression.h"

class Addition : public ArithmeticExpression {
    public:
        Addition() = default;
        ~Addition();
        Addition(Expression *l, Expression *r) : ArithmeticExpression(l, r) {}
        string evaluate();
        void print(); 
};


#endif
