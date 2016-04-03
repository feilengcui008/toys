#include <cstring>
#include <boost/lexical_cast.hpp>
#include "ArithmeticExpression.h"

ArithmeticExpression::ArithmeticExpression(Expression *l, Expression *r)
{
    left = l;
    right = r;
}

ArithmeticExpression::~ArithmeticExpression()
{
    if(left != nullptr)
        delete left;
    if(right != nullptr)
        delete right;
}

float ArithmeticExpression::stof(string s)
{
    return boost::lexical_cast<float>(s);
}

string ArithmeticExpression::ftos(float f)
{
    return boost::lexical_cast<string>(f);
}





