//
//  Attribute.h
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/27/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Attribute_h
#define Attribute_h

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




#endif /* Attribute_h */
