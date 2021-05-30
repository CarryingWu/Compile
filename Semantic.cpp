#include "Semantic.h"
#include <iostream>
using namespace std;

SemanticAnalyser::SemanticAnalyser(SyntaxTreeNode *root) {
    this->parseTreeRoot = root;
}

SemanticAnalyser::~SemanticAnalyser() {}

void SemanticAnalyser::semantics() {
    semantics(parseTreeRoot);
}

void SemanticAnalyser::semantics(SyntaxTreeNode* root) {

    vector<SyntaxTreeNode *> childs = root->childTrees;
    if (childs.size()>0){
        for (int i = childs.size()-1; i >= 0; i--) {            
            if (childs[i]->getValue().word=="VARLIST"){
                cout<<"遇到了声明语句VARLIST"<<endl;
                registerVar(childs[i]);
            }else if (childs[i]->getValue().word=="ASSIGN"){
                cout<<"遇到了赋值语句"<<endl;
                checkAssign(childs[i]);  
            }else if (childs[i]->getValue().word=="EXP"){
                cout<<"遇到了表达式EXP"<<endl;
                checkExperment(childs[i]);
            }else{
                semantics(childs[i]);
            }
        }        
    }
}


ValType SemanticAnalyser::registerVar(SyntaxTreeNode* varDeclNode){
    if (varDeclNode->getChild("VARLIST") == nullptr){ //到了最前面的声明语句了
        // cout<<"到了最底部了"<<varDeclNode->getChilds()[1]->getValue().word<<endl;
        SyntaxTreeNode * typeNode = varDeclNode->getChilds()[1];
        SyntaxTreeNode * valNode = varDeclNode->getChilds()[0];
        
        Token type = typeNode->getValue();
        Token val = valNode->getValue();
        //检查是否已经有相同定义的了
        if (symTable.findSym(val.word) != nullptr){
            cout<<"第"<<val.line<<"行出现语义错误，变量重复定义，出错变量："<<val.word<<endl;
            exit(0);
        }        
        ValType valType;
        if (type.word=="int"){
            valType = INT;
        }else if(type.word == "bool"){
            valType = BOOL;
        }else{
            valType = FLOAT;
        }        
        Sym *sym = new Sym();
        sym->tk = &val;
        sym->valType = valType;
        symTable.insertSym(val.word,sym);
        return valType;
    }else{
        ValType valType =  registerVar(varDeclNode->getChilds()[2]);
        Token tk = varDeclNode->getChilds()[0]->getValue();
        if (symTable.findSym(tk.word) != nullptr){
            cout<<"第"<<tk.line<<"行出现语义错误，变量重复定义，出错变量："<<tk.word<<endl;
            exit(0);
        }
        Sym *sym = new Sym();
        sym->tk =&tk;
        sym->valType = valType;
        symTable.insertSym(tk.word,sym);        
        return valType;
    }
}

ValType SemanticAnalyser::checkExperment(SyntaxTreeNode* expNode){
    vector<SyntaxTreeNode *> childs = expNode->childTrees;
    ValType valType = NONE;
    for (int i = childs.size()-1; i >= 0; i--) {  
        cout<<"表达式因子："<<childs[i]->getValue().word<<"类型："<<childs[i]->getValue().type<<endl;
        if (childs[i]->getValue().word=="EXP"){
            ValType tmp = checkExperment(childs[i]);
            if (valType ==NONE){
                valType = tmp;           
            }else{
                if (valType != tmp){                    
                    cout<<"第"<<childs[i]->getValue().line<<"行出现语义错误，表达式类型不匹配"<<endl;
                    exit(0);
                }
            }
        }else if (childs[i]->getValue().type=="const"){
            if (valType ==NONE){
                valType =  INT;           
            }else{
                if (valType != INT){
                    cout<<"第"<<childs[i]->getValue().line<<"行出现语义错误，表达式类型不匹配"<<endl;                    
                    exit(0);
                }
            }
        }else if (childs[i]->getValue().type=="identifier"){
            string word = childs[i]->getValue().word;
            Sym *sym =symTable.findSym(word);
            if (sym==nullptr){                
                cout<<"第"<<childs[i]->getValue().line<<"行出现语义错误，变量未声明，出错变量："<<word<<endl;
                exit(0);
            }

            ValType tmp = sym->valType;
            if (valType ==NONE){
                valType =  tmp;           
            }else{
                if (valType != tmp){                    
                    cout<<"第"<<childs[i]->getValue().line<<"行出现语义错误，表达式类型不匹配"<<endl;
                    exit(0);
                }
            }

        }
    }
    return valType;
}


void SemanticAnalyser::checkAssign(SyntaxTreeNode* assignNode){
    vector<SyntaxTreeNode *> childs = assignNode->childTrees;
    SyntaxTreeNode* idNode = childs[2];
    SyntaxTreeNode* expNode = childs[0];
    string word = idNode->getValue().word;
    Sym *sym =symTable.findSym(word);
    if (sym==nullptr){        
        cout<<"第"<<idNode->getValue().line<<"行出现语义错误，尝试对未声明变量赋值，出错变量："<<word<<endl;
        exit(0);
    }
    ValType valType = checkExperment(expNode);
    if (sym->valType != valType){        
        cout<<"第"<<idNode->getValue().line<<"行出现语义错误，变量赋值类型不匹配，出错变量："<<word<<endl;
        exit(0);
    }    
}


void SemanticAnalyser::printSymbolTableInfo(){
    cout<<"开始输出符号表";
    for (int i=0;i<20;i++){
        cout<<"=";
    }
    cout<<endl;
    symTable.displayTable();
}