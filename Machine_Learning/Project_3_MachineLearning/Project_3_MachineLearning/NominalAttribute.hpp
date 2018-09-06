//
//  NominalAttribute.hpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef NominalAttribute_hpp
#define NominalAttribute_hpp

#include "Attribute.hpp"
#include "string.h"
#include <vector>
#include <cstdio>

class NominalAttribute : public Attribute {
    
    friend ostream &operator<<( ostream &out, const NominalAttribute &na );
    
private:
    
    vector<string> domain; // types of tires
    
public:
    
    NominalAttribute();
    
    NominalAttribute( string name ) : Attribute(name) {}; // tire
    
    void addValue( string value ) throw ( logic_error ); // push type of tire onto domain
    
    int getSize() const;
    
    string getValue( int index ) throw ( logic_error );
    
    virtual int getIndex( string value ) throw ( logic_error );
    
    virtual void print() const;
    
}; // NominalAttribute class

#endif /* NominalAttribute_hpp */
