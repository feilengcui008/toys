#include <vector>
#include <cstdio>
#include <string>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <cerrno>
/*
 *
 * a simple tokenizer for c programming languages 
 * there are still many hidden bugs and haven't supported all c standard
 * maybe improve it from time to time, just for practice
 * for convenience I did not divide this file into diff modules 
 *
 * feilengcui008@gmail.com 03/11/2015
 *
 *
 * */

enum Type {
    //flag for end of token stream 
    DEFAULT = 0,

    //arithmatic 
    ADD,
    MINUS,
    MULTIPLY,
    DIVIDE,
    REMAIN,
    EQUAL,

    ARRAY_LEFT,
    ARRAY_RIGHT,

    //relation 
    RELATION_AND,
    RELATION_OR,
    RELATION_NOT,
    RELATION_LT,
    RELATION_BT,

    //binary operations 
    BINARY_AND,
    BINARY_OR,
    BINARY_NOT,
    BINARY_XOR,

    //literal constants and numbers 
    CONSTANT_LITERAL,
    CONSTANT_NUMBER,
    CONSTANT_CHAR,

    //identifiers and keywords 
    IDENTIFIER,
    KEYWORD,

    //special token like { } ( ) ; , 
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    SEMICOLON,
    ENDOFLINE,
    SHARP,
    DOT,

};

struct Token {
    Type type_; //token type 
    std::string value_; //token string value 
    int lineno_; //line number 
    int col_; //column number 
    Token():type_(DEFAULT),value_(std::string("")){}
    Token(Type type, std::string value):type_(type),value_(value){}
    ~Token(){}
};

//for convinence when handle a char 
//some tokens for now 
const std::vector<char> specialChars { '+', '-', '*', '/', '%', '=', '&', '|', '~', '^', '!', '<', '>', 
                                      '[', ']', '{', '}', '(', ')', '#',  ',', ';', '.', '\n' };
const std::vector<std::string> keywords { "char", "short", "int", "long", "unsigned", "signed", "float",
                                          "double", "union", "enum", "struct", "const",
                                          "if", "else", "switch", "case", "for", "while", "do", "continue", "break",
                                          "goto", "return" };


//some check functions for char 
bool isNumber(char c)
{
    return (c >= '0' && c <= '9');
}

bool isA(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool is_(char c)
{
    return c == '_';
}

bool isSpecialChars(char c)
{
    return std::find(specialChars.begin(), specialChars.end(), c) != specialChars.end();
}

bool isKeywords(std::string s)
{
    return std::find(keywords.begin(), keywords.end(), s) != keywords.end();
}

bool isBlank(char c)
{
    return c == '\t' || c == ' ' || c == '\r';
}


//lexer class 
class Lexer {
    public:
        Lexer():stream_(NULL), buf1_(NULL), buf2_(NULL), currentBuf_(1), lineno_(1), col_(0), current_(NULL), next_(NULL){};
        int init(FILE *stream=stdout, int buf_size=1024);
        ~Lexer();

    public:
        Token getNextToken(); //core method
        int getNextChar(char *c); //get the next char from the stream 
        int getLineno() const;
        int getCol() const;

    public:
        //Token matchStructureToken(char c); //mainly for { } ( ) , ; etc
        //Token matchArithmaticToken(char c); //mainly for binary ops, relation ops, arithmatic ops etc 
        Token matchSpecialToken(char c);//maily for tokens like + - * / { } etc 
        Token matchIdentifierToken(char c); //mainly for ID and keywords
        Token matchConstantToken(char c); //mainly for number and literal constants

    private:
        FILE *stream_; //where to get chars 
        char *buf1_; //two buffer to avoid disk io too many times
        char *buf2_;
        int bufSize_; //buf_size 
        int currentBuf_; //current in which buf 
        char *current_; //current char of input stream 
        char *next_; //next char of input stream for look ahead
        int lineno_; //current line number 
        int col_; //current column number 
};

//we alloc memory for bufs, read the first chunk of buffer from stream 
//and  set the char pointer position 
int Lexer::init(FILE *stream, int buf_size)
{
    int ret = 0;
    stream_ = stream;
    bufSize_ = buf_size;
    //alloc memory for our buffer 
    buf1_ = (char *)malloc(sizeof(char)*(buf_size+1));
    buf2_ = (char *)malloc(sizeof(char)*(buf_size+1));

    if(!buf1_ || !buf2_) {
        fprintf(stderr, "%s\n", "alloc memory for buf1_ and buf2_ failed\n");
        return (ret = -ENOMEM);
    }

    //init state,we fill the buf1_
    ret = fread(buf1_, sizeof(char), buf_size, stream);
    //fprintf(stdout, "%s\n", "==============");
    //fwrite(buf1_, sizeof(char), buf_size, stdout);
    //fprintf(stdout, "%s\n", "==============");
    if(ret<=0) {
        fprintf(stderr, "%s\n", "init read input stream failed or no char in stream\n");
        return ret;
    }
    buf1_[ret] = EOF;
    //fprintf(stderr, "=====%c=====\n", buf1_[ret]);
    current_ = buf1_;
    currentBuf_ = 1;
    return ret;
}


//destruct to release the bufs memory 
Lexer::~Lexer()
{
    if(buf1_){
        free(buf1_);
        buf1_ = NULL;
    }
    if(buf2_){
        free(buf2_);
        buf2_ = NULL;
    }
}

int Lexer::getLineno() const
{
    return this->lineno_;
}

int Lexer::getCol() const 
{
    return this->col_;
}

//get netx char from the input stream 
//check the EOF flag 
int Lexer::getNextChar(char *c)
{

    *c = *current_;
    //fprintf(stderr, "------%c---------\n", *c);

    //our fread len will never bigger than this 
    int ret = bufSize_+1;
    //handle our EOF flag for buf1_ and buf2_ and the real EOF 
    if(*c==EOF) {
        fprintf(stderr, "%s\n", "reach a EOF");
        if(currentBuf_==1) {
            ret = fread(buf2_, sizeof(char), bufSize_, stream_);
            //nothing to read 
            if(ret<=0) {
                fprintf(stderr, "%s\n", "in getNextChar 1:read error or nothing to read from stream to buf2\n"); 
                return ret;
            } else {
                //set the flag 
                buf2_[ret] = EOF;
                //change current_ pointer 
                current_ = buf2_;
                //reassign the char value 
                *c = *current_;
                //move current_ pointer to next step 
                current_++; 
            }
        } else {
            ret = fread(buf1_, sizeof(char), bufSize_, stream_);
            //nothing to read 
            if(ret<=0) {
                fprintf(stderr, "%s\n", "in getNextChar 2:read error or nothing to read from stream to buf1\n"); 
                return ret;
            } else {
                buf1_[ret] = EOF;
                current_ = buf2_;
                *c = *current_;
                current_++; 
            }
        }
    } else {
        current_++;
        col_++;
        if(*c=='\n') {
            lineno_++;
            col_ = 0;
        }
    }
    return ret;    
}


Token Lexer::getNextToken()
{
    Token endToken = Token();
    char c;
    int ret;

    //we need to skip blank
    for(;;) {
        ret = getNextChar(&c);
        if(ret<=0) {
            fprintf(stderr, "%s\n", "end scanning the input stream\n");
            return endToken;
        }
        if(!isBlank(c))
            break;
    }

    //ok, we reach the first non blank char
    /*
    ret = getNextChar(&c);
    if(ret<=0) {
        fprintf(stderr, "%s\n", "end scanning the input stream\n");
        return endToken;
    }
    */

    if(isA(c) || is_(c)) {
        return matchIdentifierToken(c); //for identifier and keywords
    } else if (isSpecialChars(c)) {  //for arithmatic and structure tokens 
        return matchSpecialToken(c);
    } else if (isNumber(c) || c=='\'' || c=='\"') {
        return matchConstantToken(c); //for number and char and string 
    }

    //default token to mark the end of token stream
    return Token();
}


//for identifier and keywords token 
Token Lexer::matchIdentifierToken(char c)
{
    Token endToken = Token();
    endToken.lineno_ = lineno_;
    endToken.col_ = col_;

    std::string temp;
    char cr;
    int ret;
    temp.push_back(c);
    for(;;) {
        ret = getNextChar(&cr);
        if(ret <= 0) {
            return endToken; //end of token stream 
        }
        if(!isA(cr) && !is_(cr) && !isNumber(cr)) {
            //a little awkward here
            current_--;
            break; 
        } else {
            temp.push_back(cr);
        }
    }
    if(isKeywords(temp)) {
        Token tk(KEYWORD, temp);
        tk.col_ = col_;
        tk.lineno_ = lineno_;
        return tk;
    } else {
        Token tk(IDENTIFIER, temp);
        tk.col_ = col_;
        tk.lineno_ = lineno_;
        return tk;
    }
}


//for some special tokens like + - * / % { } etc 
Token Lexer::matchSpecialToken(char c)
{
    Token tk = Token();
    tk.lineno_ = lineno_;
    tk.col_ = col_; 
    switch(c) {
        //actually if we combine the + , - , * ,/ to arithmatic ops, the judge is still neccesary 
        //in the parser, so we just split the ops and judge here
        case '+':
            tk.type_ = ADD;
            tk.value_ = "+"; //actually we do not need the value 
            break;
        case '-':
            tk.type_ = MINUS;
            tk.value_ = "-";
            break;
        case '*':
            tk.type_ = MULTIPLY;
            tk.value_ = "*";
            break;
        case '/':
            tk.type_ = DIVIDE;
            tk.value_ = "/";
            break;
        case '%':
            tk.type_ = REMAIN;
            tk.value_ = "%";
            break;
        case '=':
            tk.type_ = EQUAL;
            tk.value_ = "=";
            break;
        case '!':
            tk.type_ = RELATION_NOT;
            tk.value_ = "!";
            break;
        case '&':
            tk.type_ = BINARY_AND;
            tk.value_ = "&";
            break;
        case '|':
            tk.type_ = BINARY_OR;
            tk.value_ = "|";
            break;
        case '~':
            tk.type_ = BINARY_NOT;
            tk.value_ = "~";
            break;
        case '^':
            tk.type_ = BINARY_XOR;
            tk.value_ = "^";
            break;
        case '<':
            tk.type_ = RELATION_LT;
            tk.value_ = "<";
            break;
        case '>':
            tk.type_ = RELATION_BT;
            tk.value_ = ">";
            break;
        case '[':
            tk.type_ = ARRAY_LEFT;
            tk.value_ = "[";
            break;
        case ']':
            tk.type_ = ARRAY_RIGHT;
            tk.value_ = "]";
            break;
        case '{':
            tk.type_ = LEFT_BRACE;
            tk.value_ = "{";
            break;
        case '}':
            tk.type_ = RIGHT_BRACE;
            tk.value_ = "}";
            break;
        case '(':
            tk.type_ = LEFT_BRACKET;
            tk.value_ = "(";
            break;
        case ')':
            tk.type_ = RIGHT_BRACKET;
            tk.value_ = ")";
            break;
        case '\n':
            tk.type_ = ENDOFLINE;
            tk.value_ = "\n";
            break;
        case ',':
            tk.type_ = COMMA;
            tk.value_ = ",";
            break;
        case ';':
            tk.type_ = SEMICOLON;
            tk.value_ = ";";
            break;
        case '#':
            tk.type_ = SHARP;
            tk.value_ = "#";
            break;
        case '.':
            tk.type_ = SHARP;
            tk.value_ = ".";
            break;
    };
    return tk;
}


//for number and char and literal string tokens 
Token Lexer::matchConstantToken(char c)
{
    Token tk = Token();
    tk.lineno_ = lineno_;
    tk.col_ = col_;
    char cr;
    int ret;
    std::string temp;
    temp.push_back(c);
    //handle single char
    if(c=='\'') {
        ret = getNextChar(&cr);
        if(ret <= 0) {
            fprintf(stderr, "end scanning input stream or error happens at line:%d col:%d\n", lineno_, col_);
            return tk;
        }
        temp.push_back(cr);
        ret = getNextChar(&cr);
        if(ret <= 0 || c!='\'') {
            fprintf(stderr, "the char is not expressed correctly at line:%d col:%d\n", lineno_, col_);
            return tk;
        }
        temp.push_back(cr);
        tk.type_ = CONSTANT_CHAR;
        tk.value_ = temp;
        return tk;
    }

    //handle literal string 
    if(c=='\"') {
        //loop until we reach a "
        for(;;) {
            ret = getNextChar(&cr);
            if(ret <=0 ) {
                fprintf(stderr, "end scanning input stream or error happens at line:%d col:%d\n", lineno_, col_);
                return tk;
            }
            temp.push_back(cr);
            if(cr=='\"') {
                break;
            }
        }
        tk.type_ = CONSTANT_LITERAL;
        tk.value_ = temp;
        return tk;
    }

    //handle number, here we use atoi and atof for convinience 
    for(;;) {
        ret = getNextChar(&cr);
        if(ret <= 0) {
            fprintf(stderr, "end scanning input stream or error happens at line:%d col:%d\n", lineno_, col_);
            return tk;
        }
        if(cr!='.' && !isNumber(cr)) {
            break;
        }
        temp.push_back(cr);
    }
    //fprintf(stdout, "--------------number temp string%s\n", temp.data());
    tk.type_ = CONSTANT_NUMBER;
    tk.value_ = temp;    
    return tk;
}


void printToken(Token &tk)
{
    if(tk.type_==ENDOFLINE)
        fprintf(stdout, "token type:%d\ttoken value:%s\tlineno:%d\tcol:%d\n", tk.type_, "\\n", tk.lineno_, tk.col_);
    else 
        fprintf(stdout, "token type:%d\ttoken value:%s\tlineno:%d\tcol:%d\n", tk.type_, tk.value_.data(), tk.lineno_, tk.col_);
}

int main(int argc, char *argv[])
{
    //test case 
    if(argc<2) {
        fprintf(stdout, "useage:./lexer filename\n");
    }

    const char *filename = argv[1];
    Token tk = Token();
    Lexer lexer;
    //FILE *fp = fopen("threadpool.c", "r");
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        fprintf(stderr, "%s\n", "open file error");
        exit(EXIT_FAILURE);
    }
    lexer.init(fp);
    
    for(;;) {
        tk = lexer.getNextToken();
        if(tk.type_==DEFAULT) {
            fprintf(stdout, "%s\n", "token stream ended");
            break;
        } else {
            printToken(tk); 
        }
    }
    return 0;
}

