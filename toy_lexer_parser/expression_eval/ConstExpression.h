#ifndef ConstExpression_H
#define ConstExpression_H

#include <iostream>
#include "ArithmeticExpression.h"

class ConstExpression : public ArithmeticExpression {
    public:
        ConstExpression(float val);
        ~ConstExpression() = default;
        string evaluate();
        void print();
        float value;
};

#endif
