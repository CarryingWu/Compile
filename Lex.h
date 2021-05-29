//
// Created by crazywu on 2021/4/20.
//

#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include "Structure.h"

#define NKEY        5     // 关键字的个数
#define NOP       7     // 运算符的个数
#define NROP       5     // 关系运算符的个数
#define NAOP       1     // 赋值运算符的个数
#define NDEL       6     // 界限符的个数
#define NSYM   14     // 标识符的最大长度

#define MAXNUMLEN  10     // 数字的最大长度
#define MAXIDLEN   10     // 标识符的最大长度


void lexer(FILE* fp,vector<Token> *lex);

//状态state和现态currState
enum state {
    START, INNUM10,INNUM16, INID, INEQU, EQU, GTR, GEQ, NEQ, LES, LEQ, END, COMMENT
};


//定义所有的单个字符
static char csym[NSYM + 1] = {
        ' ', '+', '-', '*', '/', '&', '|', '!','=', ',', ';', '(', ')', '{', '}'
};


//关键字
static char const *word[NKEY + 1] = {
        "", /* 占位 */
        "int", "bool", "while","if", "else"
};


//全部的类别编码
enum symtype {
    SYM_NULL,SYM_INT, SYM_BOOL, SYM_WHILE, SYM_IF,SYM_ELSE,SYM_GET,SYM_PUT,SYM_IDENTIFIER,SYM_NUMBER10,SYM_NUMBER16, SYM_PLUS,SYM_SUB,SYM_MUL,SYM_DIV, SYM_AND, SYM_OR, SYM_NOT, SYM_LES, SYM_GTR, SYM_LEQ, SYM_GEQ, SYM_EQU, SYM_ASSIGN, SYM_COM, SYM_SEM, SYM_LLP, SYM_LRP, SYM_BLP, SYM_BRP
};

//关键字包含的编码
static int sym_key[NKEY + 1] = {
        SYM_NULL, SYM_INT,SYM_BOOL, SYM_WHILE, SYM_IF,SYM_ELSE
};

//运算符
static int sym_op[NOP + 1] = {//
        SYM_NULL,SYM_PLUS,SYM_SUB,SYM_MUL,SYM_DIV, SYM_AND, SYM_OR, SYM_NOT
};

//关系运算符
static int sym_rop[NROP + 1] = {//
        SYM_NULL, SYM_LES, SYM_GTR, SYM_LEQ, SYM_GEQ, SYM_EQU
};

//赋值运算符
static int sym_aop[NAOP + 1] = {//
        SYM_NULL, SYM_ASSIGN
};

//界限符
static int sym_del[NDEL + 1] = {//
        SYM_NULL, SYM_COM, SYM_SEM, SYM_LLP, SYM_LRP, SYM_BLP, SYM_BRP
};


//报错信息
static char const *err_msg[] =
        {
                /* 0 */    "Unknown character.\n",
                /* 1 */    "Incorrect symbol.\n",
                /* 2 */    "The number is too great.\n",
                /* 3 */    "The identifier is too long\n",
        };

#endif