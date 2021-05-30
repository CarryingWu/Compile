//
// Created by crazywu on 2021/4/20.
//

#include "SyntaxTreeNode.h"

#ifndef SYNTAXTREENODE_H
#define SYNTAXTREENODE_H
class SyntaxTree{
private:
	SyntaxTreeNode *root;

public:
	SyntaxTree(Token t);
	// set root node with n.
	SyntaxTree(SyntaxTreeNode *n);
	SyntaxTree();	// with root.type = ERROR
	~SyntaxTree();
	
	void display(FILE *fo=NULL);
    void bfs(FILE * fo,SyntaxTreeNode * root,int num);

    void setRootNode(Token t);
	void setRootNode(SyntaxTreeNode *n);

	void addChild(Token t);
	// add the subtree n to this tree's child3
	void addChild(SyntaxTreeNode *n);
	
	SyntaxTreeNode* getRootNode();

    vector<SyntaxTreeNode*> getChilds();
    SyntaxTreeNode* getChild(string word);


};
#endif


