#include <iostream>
#include <cstdio>

/*
 * this is just a simple template for my future toy parser, very simple one
 * the parser is now in developing
 *
 *
 * change the left recursion to right recursion
 *
 * exp -> term rest
 * rest -> + term {print('+')} rest
 *      |  - term {print('-')} rest
 *      |  null
 * term -> '0' | '1' | ... | '9' {print(digit)}
 *
 *
 * extende for divide and multiply
 *
 * */

using namespace std;

typedef char Token;

class Parser {
   public:
    Parser() : next(-1), stream(NULL) {};
    ~Parser() {
        fclose(stream);
    };
    void parseExp();
    void term();
    void rest();
    void match(Token tk);
    void printToken(Token tk);
    void setStream(FILE *s);
    Token nextToken();
    int getValue() const;

   private:
    Token next;
    FILE *stream;
    int value;
};

void Parser::parseExp() {
    next = nextToken();
    value = next - '0';
    term();
    rest();
}

void Parser::term() {
    if (next >= '0' && next <= '9') {
        printToken(next);
        match(next);
    } else {
        cout << "match term failed\n";
    }
}

void Parser::match(Token tk) {
    if (next == tk) {
        next = nextToken();
    } else {
        cout << "match token:" << tk << "error\n";
        return;
    }
}

void Parser::rest() {
    if (next == '+') {
        match('+');
        Token temp = next - '0';  // we must store for a while
        term();
        value += temp;  // we must place this op after term() check
        printToken('+');
        rest();
    } else if (next == '-') {
        match('-');
        Token temp = next - '0';
        term();
        value -= temp;
        printToken('-');
        rest();
    } else {
        // not match, we do nothing, program may ends...
    }
}

Token Parser::nextToken() { return (Token)getc(stream); }

void Parser::printToken(Token tk) { cout << tk; }

void Parser::setStream(FILE *s) { stream = s; }

int Parser::getValue() const { return value; }

int main(int argc, char *argv[]) {
    Parser p;
    FILE *fp = fopen("temp.txt", "r");
    if (fp != NULL) {
        p.setStream(fp);
        p.parseExp();
        cout << "\nvalue:" << p.getValue() << "\n";
    }
    return 0;
}
