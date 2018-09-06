//
//  Attribute.hpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <cstdio>

#include <string>
#include <iostream>

using namespace std;

class Attribute
{
    
    friend ostream& operator<<(ostream & os, const Attribute & a);
    
protected:
    
    string name;
    
public:
    
    Attribute(); // Default Constructor
    
    ~Attribute();
    
    Attribute(string name); // Explicit Constructor
    
    string getName() const;
    
    int getSize();
    
    void setName(string name);
    
    virtual void print() const;
    
    virtual int getIndex(string value);
    
}; //Attribute class

#endif /* Attribute_hpp */
