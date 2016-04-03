#include <iostream>
#include <string>
#include <cstdio>

/*
 * a simple regular expression engine for . ^ $ *
 * 
 */

using namespace std;

//global match func 
bool match(char *re, char *text)
{
    if (re == nullptr || text == nullptr)
        return false;    
    if(re[0] == '^') {
        if(_matchHead(re + 1, text)) {
            return true;
        }
    }
    
    return false;
}

bool _matchHead(char *re, char *text)
{

}

bool _matchStar(char before_star, string re, string text)
{

}

int main(int argc, char *argv[])
{
    
    return 0;
}
