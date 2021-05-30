//
// Created by crazywu on 2021/5/15.
//

#ifndef LEX_GRAMMAR_H
#define LEX_GRAMMAR_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include "SyntaxTree.h"

using namespace std;

const int state_maxn = 300;
const int guiyue = 1<<20;
const int acc = 1<<31-1;
struct Node
{
    string left;
    vector<string> right;
    set<string> search;
    int position;

    Node(string left, vector<string> right, set<string> search, int position):
            left(left), right(right), search(search), position(position) {}

    bool operator < (Node const &other) const
    {
        if (position != other.position)
            return position < other.position;
        if (left != other.left)
            return left < other.left;
        if (right != other.right)
        {
            if (right.size() != other.right.size())
                return right.size() < other.right.size();
            for (int i = 0; i < right.size(); i++)
                if (right[i] != other.right[i])
                    return right[i] < other.right[i];
        }
        if (search != other.search)
            return search < other.search;
        return false;
    }

    bool operator == (Node const &other) const
    {
        if (other.left == left && other.right == right &&
            other.search == search && other.position == position)
            return true;
        else
            return false;
    }
};

class Grammar {
private:
    map<string, int> Action[state_maxn], Goto[state_maxn];
    vector<set<Node> > I;
    vector<Node> init;
    set<string> vn, vt, vnt;
public:
    Grammar();
    virtual ~Grammar();

    void Init();
    void printInit();
    set<string> dfs(vector<string> &afterset);
    set<Node> Derive(Node &pre);
    void getProjectset();
    bool judge(vector<Token> &lex,SyntaxTree* syntaxTree);
};


#endif //LEX_GRAMMAR_H
