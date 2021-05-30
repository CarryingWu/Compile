#include <map>
#include <iostream>
#include "Structure.h"
using namespace std;

typedef enum{
    INT,BOOL,FLOAT
}ValType;

typedef struct
{
    /**符号本身**/
    Token *tk;
    /**符号对象类型：函数/变量/常量**/
    //ObjType objType;
    /**值的类型：int/bool/string**/
    ValType valType;
} Sym;

class SymTable
{
private:
    //std::vector<Sym *> sym_table;
    std::map<std::string, Sym *> sym_table;

public:
    explicit SymTable() = default;

    Sym *insertSym(const std::string &name, Sym * sym)
    {        
        sym_table.insert(std::make_pair(name, sym));
        return sym;
    }

    Sym *findSym(const std::string &name)
    {
        if (sym_table.find(name) != sym_table.end())
        {
            return sym_table[name];
        }
        else
        {
            return nullptr;
        }
    }

    void delSym(const std::string &name)
    {
        sym_table.erase(name);
    }

    void displayTable(){
        map<std::string, Sym *>::iterator iter;
        iter = sym_table.begin();
        while(iter != sym_table.end()) {
            cout << iter->first << " : " << iter->second->valType << endl;
            iter++;
        }
    }
};
