//
// Created by crazywu on 2021/5/15.
//

#include "Grammar.h"

Grammar::Grammar()
{
    Init();
    getProjectset();
}

Grammar::~Grammar()
{
}

void Grammar::Init()
{
    vector<string> right_tmp;
    right_tmp.push_back("S");
    set<string> search_tmp;
    search_tmp.insert("#");
    init.push_back(Node(string("S'"), right_tmp, search_tmp, 0));
    ifstream file_in("D:/Code/C++/Compile/syntax.txt");
    string buffer;
    while (getline(file_in, buffer))
    {
        string left, tmp;
        vector<string> right;
        stringstream stringcin(buffer);
        stringcin >> left >> tmp;
        vn.insert(left), vnt.insert(left);
        while (stringcin >> tmp)
        {
            right.push_back(tmp);
            if (tmp == "@")
                continue;
            vnt.insert(tmp);
            if (tmp[0] >= 'A' && tmp[0] <= 'Z')
                vn.insert(tmp);
            else
                vt.insert(tmp);
        }
        init.push_back(Node(left, right, search_tmp, 0));
    }
    vt.insert("#"), vnt.insert("#");
//        cout<<"语法数量"<<vt.size()<<endl;
    file_in.close();
}

void Grammar::printInit()
{
    for (vector<Node>::iterator iter = init.begin(); iter != init.end(); iter++)
    {
        cout<<iter->left<<" -> ";
        for (int i = 0; i < iter->right.size(); i++)
        {
            if (i == iter->position)
                cout<<".";
            cout<<(iter->right)[i]<<' ';
        }
        cout<<",";
        for (set<string>::iterator it = iter->search.begin(); it != iter->search.end(); it++)
            cout<<*it<<' ';
        cout<<endl;
    }
}

set<string> Grammar::dfs(vector<string> &afterset)
{
    set<string> firstset;
    for (int i = 0; i < afterset.size(); i++)
    {
        string after = afterset[i];
        if (vt.find(after) != vt.end() || after == "@")
        {
            firstset.insert(after);
            return firstset;
        }
        bool flag = true;
        for (int j = 0; j < init.size(); j++)
        {
            if (init[j].left == after)
            {
                if (init[j].right[0] == after)
                    continue;
                set<string> first = dfs(init[j].right);
                if (first.find("@") == first.end())
                {
                    flag = false;
                    firstset.insert(first.begin(), first.end());
                }
                else
                {
                    flag = true;
                    if ((i + 1) == afterset.size())
                        firstset.insert(first.begin(), first.end());
                    else
                    {
                        for (set<string>::iterator iter = first.begin(); iter != first.end(); iter++)
                        {
                            if (*iter != "@")
                                firstset.insert(*iter);
                        }
                    }
                }

            }
        }
        if (flag == false)
            break;
    }
    return firstset;
}

set<Node> Grammar::Derive(Node &pre)
{
    set<Node> derivation;
    set<string> first;
    derivation.insert(pre);
    string next = pre.right[pre.position];
    if (vn.find(next) == vn.end() || next == "@")
        return derivation;
    if ((pre.position + 1) < pre.right.size())
    {
        vector<string> after;
        for (int i = pre.position + 1; i < pre.right.size(); i++)
            after.push_back(pre.right[i]);
        first = dfs(after);
        if (first.count("@") > 0)
        {
            first.insert(pre.search.begin(), pre.search.end());
            first.erase("@");
        }
    }
    else
    {
        first.insert(pre.search.begin(), pre.search.end());
        for (set<string>::iterator iter = pre.search.begin(); iter != pre.search.end(); iter++)
            first.insert(*iter);
    }
    for (int i = 0; i < init.size(); i++)
    {
        if (init[i].left == next)
            derivation.insert(Node(next, init[i].right, first, 0));

    }
    return derivation;
}

void Grammar::getProjectset()
{
    Node accept = init[0];
    accept.position++;
    set<Node> I0 = Derive(init[0]);
    while (true)
    {
        int len = I0.size();
        for (set<Node>::iterator iter = I0.begin(); iter != I0.end(); iter++)
        {
            if (iter->position < iter->right.size())
            {
                Node tmp = *iter;
                set<Node> derivation = Derive(tmp);
                I0.insert(derivation.begin(), derivation.end());
            }
        }
        if (len == I0.size())
            break;
    }

    for (set<Node>::iterator iter = I0.begin(); iter != I0.end(); iter++)
    {
        if (iter->right[0] == "@")
        {
            Node tmp = *iter;
            int id = 0;
            for (; id < init.size(); id++)
                if(init[id].left == tmp.left && init[id].right == tmp.right)
                    break;
            set<string> search = iter->search;
            for (set<string>::iterator it = search.begin(); it != search.end(); it++)
                Action[0][*it] = id+guiyue;
        }
    }
    I.push_back(I0);
    for (int i = 0; i < I.size(); i++)
    {
        set<Node> iset = I[i];
        for (set<string>::iterator iter = vnt.begin(); iter != vnt.end(); iter++)
        {
            set<Node> niset;
            for (set<Node>:: iterator it = iset.begin(); it != iset.end(); it++)
            {
                if (it->position < it->right.size() && (it->right[it->position]) == (*iter))
                    niset.insert(Node(it->left,it->right, it->search, it->position+1));
            }

            while (true)
            {
                int len = niset.size();
                for (set<Node>:: iterator it = niset.begin(); it != niset.end(); it++)
                {
                    if (it->position < it->right.size())
                    {
                        Node tmp = (*it);
                        set<Node> derivation = Derive(tmp);
                        niset.insert(derivation.begin(), derivation.end());
                    }
                }
                if (len == niset.size())
                    break;
            }
            if (niset.empty())
                continue;

            bool isnew = true;
            for (int j = 0; j < I.size(); j++)
            {
                if (I[j] == niset)
                {
                    isnew = false;
                    if (vt.find(*iter) != vt.end())
                        Action[i][*iter] = j;
                    else
                        Goto[i][*iter] = j;
                    break;
                }
            }

            if (isnew)
            {
                I.push_back(niset);
                if (vt.find(*iter) != vt.end())
                    Action[i][*iter] = I.size()-1;
                else
                    Goto[i][*iter] = I.size()-1;
                if (niset.find(accept) != niset.end())
                    Action[I.size()-1]["#"] = acc;
                else
                {
                    for (set<Node>::iterator it = niset.begin(); it != niset.end(); it++)
                    {
                        if (it->position == it->right.size() || it->right[0] == "@")
                        {
                            Node tmp = *it;
                            int id = 0;
                            for (; id < init.size(); id++)
                                if(init[id].left == tmp.left && init[id].right == tmp.right)
                                    break;
                            for (set<string>::iterator t = it->search.begin(); t != it->search.end(); t++)
                                Action[I.size()-1][*t] = id+guiyue;
                        }
                    }
                }
            }
        }

    }
}

bool Grammar::judge(vector<Token> &lex, SyntaxTree* syntaxTree)
{
    stack<int> status;
    stack<string> symbol;
    status.push(0);
    stack<SyntaxTreeNode*> printStack;    
    while (lex.size() > 0)
    {
        Token frolex = lex.back();
        int topsta = status.top();
        cout<<"当前状态："<<topsta<<"  下一个token："<<frolex.word<<"  剩余token数量: "<<lex.size()<<endl;
        if (Action[topsta][frolex.word] == acc)
        {
            cout<<"--------->>全部token规约完毕，程序符合文法要求!"<<endl;
            cout<<"开始保存语法树"<<endl;
            syntaxTree->display();
            return true;
        }
        if (Action[topsta][frolex.word] != 0 || Action[topsta][frolex.type] != 0)
        {
            int number = max(Action[topsta][frolex.word], Action[topsta][frolex.type]);
            if (number >= guiyue)
            {
                int r = number - guiyue;
                Node pre = init[r];
                stack<string> guiyueStack;
                if (pre.right[0] != "@")
                {
                    //保存token到打印栈
                    for (int i = 0; i < pre.right.size(); i++){
                        guiyueStack.push(symbol.top());
                        symbol.pop(), status.pop();
                    }

                }
                symbol.push(pre.left);
                SyntaxTreeNode* newNode = new SyntaxTreeNode(*new Token(pre.left,"#",0));
                syntaxTree->setRootNode(newNode);
                cout<<"进行规约：";
                while (!guiyueStack.empty()){
                    cout<<guiyueStack.top()<<"  ";
                    guiyueStack.pop();
                }
                cout<<"--> "<<pre.left<<endl;

                for (int i = 0; i < pre.right.size(); ++i) {
                    syntaxTree->addChild(printStack.top());
                    printStack.pop();
                }
                printStack.push(newNode);


                if (Goto[status.top()][pre.left] == 0)
                {
                    cout<<"第"<<frolex.line<<"行出现语法错误，出错token："<<frolex.word<<"  token类型："<<frolex.type<<endl;
                    return false;
                }
                else
                    status.push(Goto[status.top()][pre.left]);
            }
            else
            {
                lex.pop_back();
                symbol.push(frolex.word);
                status.push(number);
                printStack.push(new SyntaxTreeNode(frolex));
            }
        }
        else
        {
            cout<<"第"<<frolex.line<<"行出现语法错误，出错token："<<frolex.word<<"  token类型："<<frolex.type<<endl;
            return false;
        }
    }
}