//
//  Runtime_Stack.h
//  RuM
//
//  Created by Konrad Rauscher on 11/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Runtime_Stack_h
#define Runtime_Stack_h

#include "DataTypes.h"
#include "SymbolTables.h"


class Object
{
private:

    
public:
    fun_Table member_functions;
    var_Table member_variables;
    
    string name;
    
    Object() {};
    Object(string _name) {name = _name;};
    ~Object() {};
    
    string toString();     // seemingly returns a <string> containing the variable name of the calling object.
    Val get(string);          // returns the value of member variable with name <string>.
    void set(string, Val); // sets the calling objects member named <string> with value val.
    
    Object operator=(Object const & rhsObj);
};

string Object::toString()
{
    return this->name;
}

Val Object::get(string name)
{
    return this->member_variables.get(name);
}

void Object::set(string member_name, Val value)
{
    if (value.lexeme.compare("var") == 0) { // var
        this->member_variables.get(member_name).type = value.type;
        this->member_variables.get(member_name).value = value.value;
    }
    else // literal
    {
        this->member_variables.get(member_name).type = value.lexeme;
        this->member_variables.get(member_name).value = value.token;
    }
}

Object Object::operator=(Object const & rhsObj)
{
    if (this != &rhsObj)
    {
        this->member_functions = rhsObj.member_functions;
        this->member_variables = rhsObj.member_variables;
    }
    
    return *this;
}





// NEED TO:
// update

// need to add to obj manip

// need to add to var assign

// lists --> can have lists comprising of list elements -> each list element is either a list or a Val
// in var assign, would add new list to listTable, and would assign the name of the variable...
// Q: need to be able to display lists? so for list of lists x, x[0]
// name and size instead

struct list_element {
public:
    vector<Val> list;
};

class List // so each list is a list of lists, 
{
public:
    vector<Val> element;
    
    vector<List> list;
    
    string toString();
    string & get(int);
    void set(int, Val);
};

void List::set(int, Val)
{
    // if int negative, throw runtime error
    
    // if int larger than current size of list, then increase size of the list and "zero-fill" the internal vacant spots
}






class class_Table
{
private:
    map<string, Object> table;
public:
    
    class_Table() {};
    ~class_Table() {};
    
    void add(Object newClass);
    
    bool has(string name);
    
    Object & get(string name);
    
    int size() { return table.size(); }
};

void class_Table::add(Object newClass)
{
    table.insert(make_pair(newClass.name, newClass));
}

bool class_Table::has(string name)
{
    if (table.count(name) > 0)
        return true;
    else
        return false;
}

Object & class_Table::get(string name)
{
    return table.find(name)->second;
}


struct Scope
{
public:
    var_Table varTable;
    fun_Table funTable;
    class_Table classTable; // containts types of objects? (class definitions would intraface with this?)
    class_Table objectTable; // have a object_table for list of objects currently intialized? (new() would interface with this?)
    
    Scope();
    ~Scope();
};


Scope::Scope()
{
    
}

Scope::~Scope()
{
    
}

//global scope should always be an accesible scope
class Runtime_Stack
{
private:
    vector<Scope> stack;

public:
    string mode;
    
// global scope
//    Scope global_scope;
    
// stack of scopes
    Runtime_Stack();
    ~Runtime_Stack();
    
//    pop
    void pop();
    
    Scope & top(); // ISSUE WITH REFERENCE RETURN VALUE???
    
    Scope & global();
    
    void push_on();
    
    bool hasVar(string varName);
    bool hasFun(string funName);
    bool hasClass(string className);
    
    Val getVar(string varName);
    Val getVar_aboveScopes(string varName);
    
    void updateVar(string varName, Val val);
    
    fun_Table_element getFun(string varName);
    
        // load all functions, variables into table
    
    
    // function that checks appropriate scopes for var or function 
    
};

Runtime_Stack::Runtime_Stack()
{
    this->mode = "regular";
}

Runtime_Stack::~Runtime_Stack()
{
    stack.clear();
}

//    pop
void Runtime_Stack::pop()
{
    stack.pop_back();
}

Scope & Runtime_Stack::top()
{
    return stack.back();
}

Scope & Runtime_Stack::global()
{
    return stack[0];
}

void Runtime_Stack::push_on()
{
    Scope scope = * new Scope;
    
    if (this->stack.size() >= 1) {
        scope.funTable = this->top().funTable;
        scope.varTable = this->top().varTable;
        scope.classTable = this->top().classTable;
        scope.objectTable = this->top().objectTable;
    }
    
    
    
    
    stack.push_back(scope);
}

bool Runtime_Stack::hasVar(string varName)
{

    // look in all indixes, also global
    
    bool result = false;
    
    for (int i = 0; i < this->stack.size(); i++)
    {
        if (this->stack[i].varTable.has(varName))
        {
            result = true;
        }
    }
    
//    if (this->global_scope.varTable.has(varName)) {
//        result = true;
//    }
//    
    return result;
}

bool Runtime_Stack::hasFun(string funName)
{
    // look in all indixes, also global

    bool result = false;
    
    for (int i = 0; i < this->stack.size(); i++)
    {
        if (this->stack[i].funTable.has(funName))
        {
            result = true;
        }
    }
    
//    if (this->global_scope.funTable.has(funName)) {
//        result = true;
//    }
    
    return result;
}

//Runtime_Stack::getFun(string funName)
//{
//    
//}

Val Runtime_Stack::getVar_aboveScopes(string varName)
{
    Val result;
    // start at end of vector of scopes, keep going up until varName found
    
    for (int i = (this->stack.size() - 2); i >= 0; i--)
    {
        if(this->stack[i].varTable.has(varName))
        {
            
            Val var = this->stack[i].varTable.get(varName);
            
            if (var.hasRefVar && this->hasVar(var.referenceName) && ( ! this->getVar(var.referenceName).value.compare("undefined") == 0) ) {
                Val refVar = this->getVar(var.referenceName);
                
                var.value = refVar.value;
                var.type = refVar.type;
                
                // update var in runtimestack
                
                this->updateVar(var.token, var);
            }
            
            result = this->stack[i].varTable.get(varName);
            return result;
        }
    }
    
    // if still not found, check global
//    if(this->global_scope.varTable.has(varName))
//    {
//        result = this->global_scope.varTable.get(varName);
//        return result;
//    }
    
    return result;
}

Val Runtime_Stack::getVar(string varName)
{
    Val result;
    // start at end of vector of scopes, keep going up until varName found
    
    for (int i = (this->stack.size() - 1); i >= 0; i--)
    {
        if(this->stack[i].varTable.has(varName))
        {
            
            Val var = this->stack[i].varTable.get(varName);
            
            if (var.hasRefVar && this->hasVar(var.referenceName) && ( ! this->getVar(var.referenceName).value.compare("undefined") == 0)  ) {
                Val refVar = this->getVar(var.referenceName);
                
                var.value = refVar.value;
                var.type = refVar.type;
                
                // update var in runtimestack
                
                this->updateVar(var.token, var);
            }

            result = this->stack[i].varTable.get(varName);
            return result;
        }
    }
    
//    // if still not found, check global
//    if(this->global_scope.varTable.has(varName))
//    {
//        result = this->global_scope.varTable.get(varName);
//        return result;
//    }
    
    return result;
}

void Runtime_Stack::updateVar(string varName, Val val)
{
    Val result;
    // start at end of vector of scopes, keep going up until varName found
    
    for (int i = (this->stack.size() - 1); i >= 0; i--)
    {
        if(this->stack[i].varTable.has(varName))
        {
            this->stack[i].varTable.update(varName, val);
        }
    }
}

fun_Table_element Runtime_Stack::getFun(string funName)
{
    fun_Table_element result;
    // start at end of vector of scopes, keep going up until varName found
    
    for (int i = (this->stack.size() - 1); i >= 0; i--)
    {
        if(this->stack[i].funTable.has(funName))
        {
            result = this->stack[i].funTable.get(funName);
            return result;
        }
    }
    
//    // if still not found, check global
//    if(this->global_scope.funTable.has(funName))
//    {
//        result = this->global_scope.funTable.get(funName);
//        return result;
//    }
    
    return result;
}













#endif /* Runtime_Stack_h */
