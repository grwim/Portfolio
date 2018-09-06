//
//  Examples.h
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/29/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Examples_h
#define Examples_h
#include <vector>
#include <iostream>
#include "Example.h"
#include "Attributes.h"
#include <cstdio>


// is a child of vector< example >

class Examples : public std::vector<Example>
{
    friend std::ostream &operator<<( std::ostream &out, const Examples &e );
    
private:
    
    Attributes attributes;
    
public:
    
    Examples();
    
    Examples( const Attributes &attributes ) throw ( logic_error );
    
    void parse( const vector<string> &data ) throw ( logic_error );
    
    void setAttributes( const Attributes &attributes ) throw ( logic_error );
    
    void print() const;
    
}; // Examples class
#endif /* Examples_h */
