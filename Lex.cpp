//
// Created by crazywu on 2021/4/20.
//
#include <string>
#include <vector>
#include "Lex.h"
char ch;         // 最后读出的char
int  sym;        // 读出的字
int currState = START;        //现态
void error(int line,int type)
{
    printf("Error in line %d: %s\n", line, err_msg[type]);
}



void lexer(FILE* fp,std::vector<Token> *lex)
{
    int num = 0;          //当前识别中的数字
    int k = 0;            //当前识别中的数字的长度
    char a[MAXIDLEN + 1]; //当前识别中的标识符or关键字
    int a_index = 0;      //当前识别中的标识符or关键字的下标
    int line = 1;         //当前的行数，用于错误打印
    ch = fgetc(fp); //获取文件第一个字符

    while (ch != EOF)
    {
        switch (currState)
        {
            case START:
                if (ch == ' ' || ch == '\t')
                {
                }
                else if (ch == '\r' || ch == '\n')
                {
                    line++;
                }
                else if (isdigit(ch))
                {
                    currState = INNUM10;
                    num = num * 10 + ch - '0';
                    k++;
                }
                else if (isalpha(ch))
                {
                    currState = INID;
                    if (a_index > MAXIDLEN)
                    {
                        error(line,3);
                        exit(1);
                    }
                    a[a_index] = ch;
                    a_index++;
                }
                else if (ch == '=')
                    currState = INEQU;
                else if (ch == '>')
                    currState = GTR;
                else if (ch == '<')
                    currState = LES;
                else
                { //单独字符直接识别
                    currState = START;
                    int i = 1;
                    for (; i <= NSYM; i++)
                    {
                        if (ch == csym[i])
                            break;
                    }
                    //单个的运算符
                    if (i <= NOP)
                    {
                        sym = sym_op[i];
                        printf("(%d,%c,运算符)\n", sym, ch);
                        std::string str(1,ch);
                        lex->push_back(Token(str, "operator",line));
                    }
                        //赋值运算符
                    else if (i <= NOP+NAOP)
                    {
                        sym = sym_aop[i-NOP];
                        printf("(%d,%c,赋值运算符)\n", sym, ch);
                        std::string str(1,ch);
                        lex->push_back(Token(str, "operator",line));
                    }
                        //界限符
                    else if (i <= NOP+NAOP+NDEL)
                    {
                        sym = sym_del[i-NOP-NAOP];
                        printf("(%d,%c,界限符)\n", sym, ch);
                        std::string str(1,ch);
                        lex->push_back(Token(str, "delimiter",line));
                    }
                    else
                    {
                        error(line,0);
                        exit(1);
                    }
                }
                break;
            case INNUM10:
                if (isdigit(ch))
                {
                    num = num * 10 + ch - '0';
                    k++;
                }
                else if (num==0 && ch == 'x') {
                    currState = INNUM16;
                    k = 0;
                }
                else
                { //token识别完毕
                    currState = START;
                    ch = ungetc(ch, fp); // 回退该字符，重新识别
                    sym = SYM_NUMBER10;
                    if (k > MAXNUMLEN) {
                        error(line, 2);
                        exit(1);
                    }
                    else if (isalpha(ch)) {
                        error(line, 1);
                        exit(1);
                    }
                    else
                    {
                        printf("(%d,%d,10进制常数)\n", sym, num);
                        lex->push_back(Token(std::to_string(num), "const",line));
                    }
                    k = 0;
                    num = 0;
                }
                break;
            case INNUM16:
                if (isdigit(ch))
                {
                    num = num * 16 + ch - '0';
                    k++;
                }
                else if (isalpha(ch)) {
                    num = num * 16 + (ch - 'a'+10);
                    k++;
                }
                else
                { //token识别完毕
                    currState = START;
                    ch = ungetc(ch, fp); // 回退该字符，重新识别
                    sym = SYM_NUMBER16;
                    if (k > MAXNUMLEN) {
                        error(line, 2);
                        exit(1);
                    }
                    else
                    {
                        printf("(%d,0x%x,16进制常数)\n", sym, num);
                        lex->push_back(Token(std::to_string(num), "const",line));

                    }
                    k = 0;
                    num = 0;
                }
                break;
            case INID:
                if (isalpha(ch) || isdigit(ch))
                {
                    if (a_index > MAXIDLEN)
                    {
                        error(line, 3);
                        exit(1);
                    }
                    a[a_index] = ch;
                    a_index++;
                }
                else
                { //token识别完毕
                    currState = START;
                    ch = ungetc(ch, fp); // 回退该字符，重新识别
                    a[a_index] = '\0';   // 字符数组和字符串的区别就是结尾少了\0，一定要加上！
                    // 检查是否为关键字
                    int i = 1;
                    for (; i <= NKEY; i++)
                    {
                        if (strcmp(a, word[i]) == 0)
                            break;
                    }
                    if (i <= NKEY)
                    {
                        sym = sym_key[i]; // 该符号是关键字
                        printf("(%d,%s,关键字)\n", sym, a);
                        lex->push_back(Token(a, "keyword",line));
                    }
                    else
                    {
                        sym = SYM_IDENTIFIER; // 该符号是标识符
                        printf("(%d,%s,标识符)\n", sym, a);
                        lex->push_back(Token(a, "identifier",line));
                    }
                    a_index = 0;
                }
                break;
            case INEQU:
                if (ch == '=')
                {
                    currState = EQU;
                }
                else
                {
                    currState = START;
                    ch = ungetc(ch, fp); // 回退该字符，重新识别
                    sym = SYM_ASSIGN;
                    printf("(%d,=,赋值运算符)\n", sym);
                    lex->push_back(Token("=", "assign",line));
                }
                break;
            case GTR:
                if (ch == '=')
                {
                    currState = GEQ;
                }
                else
                { //token识别完毕
                    currState = START;
                    ch = ungetc(ch, fp); // 回退该字符，重新识别
                    sym = SYM_GTR;
                    printf("(%d,>,关系运算符)\n", sym);
                    lex->push_back(Token(">", "operator",line));

                }
                break;
            case LES:
                if (ch == '=')
                {
                    currState = LEQ;
                }
                else
                { //token识别完毕
                    currState = START;
                    ch = ungetc(ch, fp); // 回退该字符，重新识别
                    sym = SYM_LES;
                    printf("(%d,<,关系运算符)\n", sym);
                    lex->push_back(Token("<", "operator",line));
                }
                break;
            case EQU: //token识别完毕
                currState = START;
                ch = ungetc(ch, fp); // 回退该字符，重新识别
                sym = SYM_EQU;
                printf("(%d,==,关系运算符)\n", sym);
                lex->push_back(Token("==", "operator",line));
                break;
            case GEQ: //token识别完毕
                currState = START;
                ch = ungetc(ch, fp); // 回退该字符，重新识别
                sym = SYM_GEQ;
                printf("(%d,>=,关系运算符)\n", sym);
                lex->push_back(Token(">=", "operator",line));
                break;
            case LEQ: //token识别完毕
                currState = START;
                ch = ungetc(ch, fp); // 回退该字符，重新识别
                sym = SYM_LEQ;
                printf("(%d,<=,关系运算符)\n", sym);
                lex->push_back(Token("<=", "operator",line));
                break;
        }

        //在最后获取下一个字符
        ch = fgetc(fp);
    }
}