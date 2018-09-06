//
//  NumericAttribute.cpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "NumericAttribute.hpp"

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
