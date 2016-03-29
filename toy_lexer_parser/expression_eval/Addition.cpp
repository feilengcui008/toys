#include <cstring>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Addition.h"

Addition::~Addition()
{
    if(left != nullptr) {
        delete left;
    }
    if(right != nullptr) {
        delete right;
    }
}

void Addition::print()
{
    if(left != nullptr)
        left->print();
    std::cout << "+";
    if(right != nullptr)
        right->print();    
}

string Addition::evaluate()
{
    float ret = stof(left->evaluate()) + stof(right->evaluate());
    return ftos(ret);
}
