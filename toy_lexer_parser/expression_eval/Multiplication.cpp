#include <cstring>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Multiplication.h"

Multiplication::~Multiplication()
{
    if(left != nullptr)
        delete left;
    if(right != nullptr)
        delete right;
}

void Multiplication::print()
{
    if(left != nullptr)
        left->print();
    std::cout << "*";
    if(right != nullptr)
        right->print();    
}

string Multiplication::evaluate()
{
    float ret = stof(left->evaluate()) * stof(right->evaluate());
    return ftos(ret);
}
