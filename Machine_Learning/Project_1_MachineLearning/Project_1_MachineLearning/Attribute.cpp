//
//  Attribute.cpp
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/30/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Attribute.h"

using namespace::std;

Attribute::Attribute()
{
    
}

Attribute::Attribute(string myName)
{
    name = myName;
}

Attribute::~Attribute()
{
    
}

string Attribute::getName()	const
{
    return name;
}

void Attribute::setName(string myName)
{
    name = myName;
}

int Attribute::getSize()
{
    // Returns an int storing the size of the domain
    return 0;
}



ostream& operator<<(ostream & os, const Attribute & a) {
    
    a.print();
    return os;
    
}

 void Attribute::print() const
{
    cout << name;
}

int Attribute::getIndex(string value)
{
    return -1;
}