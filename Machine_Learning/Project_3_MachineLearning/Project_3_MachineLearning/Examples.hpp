//
//  Examples.hpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Examples_hpp
#define Examples_hpp

#include <cstdio>
#include <vector>
#include <iostream>
#include "Example.hpp"
#include "Attributes.hpp"

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

#endif /* Examples_hpp */
