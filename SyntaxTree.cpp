//
// Created by crazywu on 2021/4/20.
//

#include <stdio.h>
#include <string.h>
#include "SyntaxTree.h"


/*======================================================================*/

SyntaxTree::SyntaxTree(Token t){
	root = new SyntaxTreeNode(t);
}
SyntaxTree::SyntaxTree(SyntaxTreeNode *n){
	setRootNode(n);
}

SyntaxTree::SyntaxTree(){
	setRootNode(new SyntaxTreeNode());
}

SyntaxTree::~SyntaxTree(){
	if(root != NULL){
		delete root;
	}
}

/*====================================================================*/


void SyntaxTree::display(FILE *fo){
	if(fo == NULL){
        fo = fopen("D:/Code/C++/Compile/SyntaxTree.txt", "w");
		//fo = stdout;
	}
    bfs(fo,this->root,0);
    fclose(fo);
}

void SyntaxTree::bfs(FILE *fo,SyntaxTreeNode* root,int num) {
    vector<SyntaxTreeNode *> childs = root->childTrees;
    if (childs.size()>0){
        for (int i = 0; i < num; ++i) {
            fprintf(fo,"  ");
        }
        num++;
        fprintf(fo,"%s -->",root->getValue().word.c_str());

        for (int i = childs.size()-1; i >= 0; i--) {
            fprintf(fo,"%s ",childs[i]->getValue().word.c_str());
        }
        fprintf(fo,"\n");
    }

    for (int i = childs.size()-1; i >= 0; i--) {
        bfs(fo,childs[i],num);
    }
}

void SyntaxTree::setRootNode(Token t){
	root = new SyntaxTreeNode(t);
}
void SyntaxTree::setRootNode(SyntaxTreeNode *n){
	if(n == NULL){
		root = new SyntaxTreeNode();
	}else{
		root = n;
	}
}


void SyntaxTree::addChild(Token t){
	root->childTrees.push_back(new SyntaxTreeNode(t));
}
void SyntaxTree::addChild(SyntaxTreeNode *n){
	if(n == NULL) return;
    root->childTrees.push_back(n);
}


SyntaxTreeNode* SyntaxTree::getRootNode(){
	return root;
}


SyntaxTreeNode* SyntaxTree::getChild(string word) {
	if(root == NULL)
		return NULL;
	else{
	    vector<SyntaxTreeNode*> child =  root->childTrees;
        for (int i = 0; i < child.size(); ++i) {
            if (child[i]->getValue().word==word){
                return child[i];
            }
        }
        return NULL;
	}

}

vector<SyntaxTreeNode*> SyntaxTree::getChilds() {
    if(root == NULL)
        return *new vector<SyntaxTreeNode*>();
    else{
        return root->childTrees;
    }

}






