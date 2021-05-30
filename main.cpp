//
// Created by crazywu on 2021/4/20.
//
#include "Lex.h"
#include "Grammar.h"
#include "Semantic.h"
#include "SyntaxTree.h"
using namespace std;
static vector<Token> lex;

int main()
{    
    //获取源文件的指针，助教老师如果要测试的话，这里的路径请改为源文件在本机的绝对路径，另外Grammar.cpp中的init函数中的文法路径也需要修改
    FILE* fp = fopen("D:/Code/C++/Compile/source.txt", "r");
    if (!fp)
    {
        cout<<"代码源文件不存在"<<endl;
        exit(0);
    }
    else{ //将待检验文件放入词法分析器进行分析
        cout<<"开始进行词法分析"<<endl;
        lexer(fp,&lex);
        cout<<"词法分析结束"<<endl;
    }

    //到这一步说明词法分析已经通过，开始进行语法分析
    //添加结束符号
    lex.push_back(Token("#", "halt",0));
    for (int i = 0, j = lex.size()-1; i < j; i++, j--)
        swap(lex[i], lex[j]);
    cout<<"token总数目："<<lex.size()<<endl;
    Grammar grammar;
    cout<<"开始进行语法分析"<<endl;
    SyntaxTree syntaxTree;
    bool isJudged = grammar.judge(lex,&syntaxTree);
    cout<<"语法分析结束"<<endl;

    if (isJudged){
        cout<<"开始进行语义分析"<<endl;
        SyntaxTreeNode * root = syntaxTree.getRootNode();
        SemanticAnalyser *semanticAnalyser = new SemanticAnalyser(root);
        semanticAnalyser->semantics();
        cout<<"分析完成"<<endl;
        semanticAnalyser->printSymbolTableInfo();
        cout<<"语义分析结束"<<endl;
    }

    return 0;
}
