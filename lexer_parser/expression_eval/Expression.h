#ifndef Expression_H

#define Expression_H

#include <string>
using namespace std;

/*
 * abstract Class for expression 
 *
 *
 */

class Expression {
    public:
        Expression() = default;
        virtual ~Expression() = default;
        virtual string evaluate() = 0;
        virtual void print() = 0;
};


#endif 
