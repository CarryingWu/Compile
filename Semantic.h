#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_
#include <vector>
#include "SyntaxTreeNode.h"
#include "SymbolTable.h"
using namespace std;

/***************************************
* 类名：    SemanticAnalyser
* 作用：    语义分析器
****************************************/
class SemanticAnalyser {
public:

	SemanticAnalyser(SyntaxTreeNode* root);

	~SemanticAnalyser();

	void semantics();
    void semantics(SyntaxTreeNode* root);

	void printSymbolTableInfo();
private:
	SyntaxTreeNode* parseTreeRoot;  // 解析树根节点
	SymTable symTable;

	ValType registerVar(SyntaxTreeNode* varDeclNode);

	ValType checkExperment(SyntaxTreeNode* expNode);

};
#endif