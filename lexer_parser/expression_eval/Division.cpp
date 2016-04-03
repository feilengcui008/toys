#include <cstring>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Division.h"

Division::~Division()
{
    if(left != nullptr)
        delete left;
    if(right != nullptr)
        delete right;
}

void Division::print()
{
    if(left != nullptr)
        left->print();
    std::cout << "/";
    if(right != nullptr)
        right->print();    
}

string Division::evaluate()
{
    float ret = stof(left->evaluate()) * 1.0 / stof(right->evaluate());
    return ftos(ret);
}
