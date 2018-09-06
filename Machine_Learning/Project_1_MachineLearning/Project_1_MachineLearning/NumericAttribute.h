//
//  NumericAttribute.h
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/29/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef NumericAttribute_h
#define NumericAttribute_h

#include <iostream>
#include <cstdio>
#include "Attribute.h"

class NumericAttribute : public Attribute {
    
    friend ostream &operator<<( ostream & os, const NumericAttribute &na );
    
public:
    
    NumericAttribute();
    
    NumericAttribute( string name ) : Attribute( name ) { };  // weight of bike
    
    
    
    virtual void print() const;
    
    virtual int getIndex(string value);

};


#endif /* NumericAttribute_h */
