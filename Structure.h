//
// Created by crazywu on 2021/5/15.
//

#ifndef LEX_STRUCTURE_H
#define LEX_STRUCTURE_H
#include <string>
using namespace std;
struct Token
{
    string word;
    string type;
    int line;
    Token(string word, string type, int line): word(word), type(type), line(line){}
};

#endif //LEX_STRUCTURE_H
