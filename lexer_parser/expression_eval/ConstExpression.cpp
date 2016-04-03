#include <boost/lexical_cast.hpp>
#include "ConstExpression.h"

ConstExpression::ConstExpression(float val)
{
    value = val;
}

void ConstExpression::print()
{
    std::cout << value; 
}

string ConstExpression::evaluate()
{
    return ftos(value);
}
