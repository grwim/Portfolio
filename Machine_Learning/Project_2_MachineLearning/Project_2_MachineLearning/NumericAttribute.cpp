//
//  NumericAttribute.cpp
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/30/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//



#include "NumericAttribute.h"


NumericAttribute::NumericAttribute()
{
    
    
    
}

ostream &operator<<( ostream & os, const NumericAttribute &na )
{

    na.print();
    return os;
    
}

 void NumericAttribute::print() const
{
    cout << name << " numeric";
}

int NumericAttribute::getIndex(string value)
{
    return -1;
}