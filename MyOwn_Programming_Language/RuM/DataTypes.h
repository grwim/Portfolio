//
//  DataTypes.h
//  RuM
//
//  Created by Konrad Rauscher on 11/7/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef DataTypes_h
#define DataTypes_h

//#include "Runtime_Stack.h"

using namespace std;

struct Val
{
    
public:
    string lexeme;
    string token;
    
    // var only
    string type;
    string value;
    
    Val();
    Val(string lex, string tok, string, string);
    ~Val() {};
    
    bool byReference;
    
    bool hasRefVar;
    string referenceName;
    
    Val operator=(Val const & rhsObj);
    
};

Val::Val()
{
    lexeme = "";
    token = "";
    
    type = "undefined";
    value = "undefined";
    
    hasRefVar = false;
    
    byReference = true;
}

Val::Val(string lex, string tok, string _value = "", string _type = "")
{
    lexeme = lex;
    token = tok;
    type = _type;
    value = _value;
}

Val Val::operator=(const Val & rhsObj)
{
    if (this != &rhsObj)
    {
        lexeme = rhsObj.lexeme;
        token = rhsObj.token;
        type = rhsObj.type;
        value = rhsObj.value;
        
        byReference = rhsObj.byReference;
        hasRefVar = rhsObj.hasRefVar;
        referenceName = rhsObj.referenceName;
    }
    
    return *this;
}


// overload assignment operator


// function



// class

    // has a vector of of member functions 




#endif /* DataTypes_h */
