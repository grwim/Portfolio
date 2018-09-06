//
//  NumericAttribute.hpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef NumericAttribute_hpp
#define NumericAttribute_hpp

#include <iostream>
#include <cstdio>
#include "Attribute.hpp"

class NumericAttribute : public Attribute {
    
    friend ostream &operator<<( ostream & os, const NumericAttribute &na );
    
public:
    
    NumericAttribute();
    
    NumericAttribute( string name ) : Attribute( name ) { };  // weight of bike
    
    
    
    virtual void print() const;
    
    virtual int getIndex(string value);
    
};

#endif /* NumericAttribute_hpp */
