//
//  SymbolTables.h
//  RuM
//
//  Created by Konrad Rauscher on 11/7/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef SymbolTables_h
#define SymbolTables_h

#include "DataTypes.h"

using namespace std;

class var_Table
{
private:
    map<string, Val> table; // name | val
    
public:
    
    var_Table();
    ~var_Table();
    
    // add
    void add(Val variable);
    
    bool has(string token);
    
    Val get(string token);
    
    void update(string token, Val variable);
    
};

var_Table::var_Table()
{
    
}

var_Table::~var_Table()
{
    this->table.clear();
}

void var_Table::add(Val variable)
{
//    Val * ptr = &variable;
    
    this->table.insert(make_pair(variable.token, variable));
}

bool var_Table::has(string token)
{
    
    if (this->table.count(token) > 0)
        return true;
    else
        return false;
}

Val var_Table::get(string token) // returns variable from map, based on name
{
    return this->table.find(token)->second;
}

void var_Table::update(string token, Val variable)
{
    if (variable.lexeme.compare("var") == 0) {
        //this->table.find(token)->second.value = variable.value;
        //this->table.find(token)->second.type = variable.type;
        
        //this->table.find(token)->second.lexeme = "TESTESTEST";
        
        this->table.find(token)->second = variable;

    }
    else
    {
        this->table.find(token)->second.value = variable.token;
        this->table.find(token)->second.type = variable.lexeme;
    }
}


struct fun_Table_element
{
public:
    string fun_name;
    vector <Val> param_list;
    vector< vector <string> > code; // token [0], lexem [1]
    string return_var;
    
    fun_Table_element() {};
    ~fun_Table_element() {param_list.clear(); code.clear(); return_var.clear(); };
    
    fun_Table_element operator=(const fun_Table_element & rhsObj);
    
};

fun_Table_element fun_Table_element::operator=(const fun_Table_element & rhsObj)
{
    if (this != &rhsObj)
    {
        fun_name = rhsObj.fun_name;
        param_list = rhsObj.param_list;
        code = rhsObj.code;
        return_var = rhsObj.return_var;
    }
    
    return *this;
}

class fun_Table
{
private:
    vector<fun_Table_element> fTable;
public:
    
    fun_Table();
    ~fun_Table();
    
    void add( string funName, vector<Val> paramList, vector < vector < string > > tokenList, string returnVar );
    
    bool has( string funName);
    
    fun_Table_element get(string funName);
    
    vector < vector<string> > getCode(string fun_name);
    vector<Val> getParamList(string fun_name);
    string getReturnVar(string fun_name);
    
    
    void print(string expr);   // which will print to screen the evaluation of <expr>
    void copy(string expr);  // which will create a deep copy of the value of <expr>
    
};

fun_Table::fun_Table()
{
    
}

fun_Table::~fun_Table()
{
    // clear all vectors
    this->fTable.clear();
}

void fun_Table::add( string funName, vector<Val> paramList, vector< vector<string> > code, string returnVar )
{
    fun_Table_element element; // = * new fun_Table_element;
    
    element.fun_name = funName;
    element.param_list = paramList;
    element.code = code;
    element.return_var = returnVar;
    
    fTable.push_back(element);
}

bool fun_Table::has( string funName)
{
    bool result = false;
    for (int i = 0; i < this->fTable.size(); i++)
    {
        if (funName == this->fTable[i].fun_name) {
            result = true;
            i = 1000000;
        }
    }
    return result;
}

vector < vector <string> > fun_Table::getCode(string funName)
{
    vector < vector<string> > result;
    for (int i = 0; i < this->fTable.size(); i++)
    {
        if (funName == this->fTable[i].fun_name) {
            result = this->fTable[i].code;
            i = 1000000;
        }
    }
    return result;
}

vector<Val> fun_Table::getParamList(string funName)
{
    vector<Val> result;
    for (int i = 0; i < this->fTable.size(); i++)
    {
        if (funName == this->fTable[i].fun_name) {
            result = this->fTable[i].param_list;
            i = 1000000;
        }
    }
    return result;
}

string fun_Table::getReturnVar(string funName)
{
    string result;
    for (int i = 0; i < this->fTable.size(); i++)
    {
        if (funName == this->fTable[i].fun_name) {
            result = this->fTable[i].return_var;
            i = 1000000;
        }
    }
    return result;
}

fun_Table_element fun_Table::get(string funName)
{
    fun_Table_element result;
    for (int i = 0; i < this->fTable.size(); i++)
    {
        if (funName == this->fTable[i].fun_name) {
            result = this->fTable[i];
            i = 1000000;
        }
    }
    return result;
}




#endif /* SymbolTables_h */
