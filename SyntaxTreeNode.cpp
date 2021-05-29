//
// Created by crazywu on 2021/4/20.
//

#include "SyntaxTreeNode.h"

SyntaxTreeNode::SyntaxTreeNode(){
    this->root = new Token("#","",0);

}

SyntaxTreeNode::SyntaxTreeNode(Token t){
    this->root = new Token(t.word,t.type,t.line);

}

SyntaxTreeNode::~SyntaxTreeNode(){
    for (int i = 0; i < childTrees.size(); ++i) {
        delete childTrees[i];
    }
    delete root;
}

/*===================================================================*/


void SyntaxTreeNode::setValue(Token val){
	this->root = new Token(val.word,val.type,val.line);;
}

Token SyntaxTreeNode::getValue(){
	return *this->root;
}


void SyntaxTreeNode::addChild(Token t) {
	childTrees.push_back(new SyntaxTreeNode(t));
}
void SyntaxTreeNode::addChild(SyntaxTreeNode *n){
	childTrees.push_back(n);
}


vector<SyntaxTreeNode*> SyntaxTreeNode::getChilds(){
	return childTrees;
}

SyntaxTreeNode* SyntaxTreeNode::getChild(string word){
    for (int i = 0; i < childTrees.size(); ++i) {
        if (childTrees[i]->getValue().word==word){
            return childTrees[i];
        }
    }
    return nullptr;
}



