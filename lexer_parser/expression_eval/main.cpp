#include <iostream>
#include <cstdio>
#include <boost/lexical_cast.hpp>
#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Division.h"
#include "ConstExpression.h"

Expression *makeTree(string str, int s, int t)
{
    while(s <= t && str[s] == '(' && str[t] == ')') {
        s++;
        t--;
    }
    if(s > t)
        return new ConstExpression(0);
    bool findChar = false; //operator
    int bracket = 0;
    int lastAdd, lastSub, lastMul, lastDiv;
    lastAdd = lastSub = lastMul = lastDiv = -1; 
    int i;
    for (i = s; i <= t; ++i) {
        if(str[i] != '.' && (str[i] < '0' || str[i] > '9')) {
            //must be operator 
            findChar = true;
            switch (str[i]) {
                case '(' : 
                    bracket++;
                    break;
                case ')':
                    bracket--;
                    break;
                case '+':
                    if(!bracket) //pop stack 
                        lastAdd = i;
                case '-':
                    if(!bracket) //pop stack 
                        lastSub = i;
                    break;
                case '*':
                    if(!bracket)
                        lastMul = i;
                    break;
                case '/':
                    if(!bracket)
                        lastDiv = i;
                    break;
                default:
                    std::cout << "there exist invalid letter\n";
            }

        } 
    }
    // find const 
    if(findChar == false){
        //std::cout << "find const:" << str.substr(s, t-s+1) << "\n";
        return new ConstExpression(boost::lexical_cast<float>(str.substr(s, t-s+1)));
    }


    //+ - must be previous to * / 
    //because * / have higher priority and should be in 
    //depper recursion, this kind of implementation is a little
    //trick, but actually the same as the right recursion of 
    //regular grammar for algorithmetic expressions 
    //find operator 
    if(lastAdd != -1) {
        //std::cout << lastAdd << "\n";
        std::cout << "find +\n";
        return new Addition(makeTree(str, s, lastAdd-1), makeTree(str, lastAdd+1, t)); 
    }

    if(lastSub != -1) {
        std::cout << "find -\n";
        return new Subtraction(makeTree(str, s, lastSub-1), makeTree(str, lastSub+1, t)); 
    }

    if(lastMul != -1) {
        std::cout << "find *\n";
        return new Multiplication(makeTree(str, s, lastMul-1), makeTree(str, lastMul+1, t)); 
    }

    if(lastDiv != -1) {
        std::cout << "find /\n";
        return new Division(makeTree(str, s, lastDiv-1), makeTree(str, lastDiv+1, t)); 
    }

    return nullptr;

}

int main(int argc, char *argv[])
{
    //string s("(1+2)*3");
    //string s("1+2*3+5.0/4");
    string s;
    while(std::cin >> s) {
        Expression *exp = makeTree(s, 0, s.length()-1);
        std::cout << "expression: ";
        exp->print(); 
        std::cout << "\n";   
        std::cout << "result: " << exp->evaluate() << std::endl;
    }
    //delete exp;
    return 0;
}
