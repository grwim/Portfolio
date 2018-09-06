//
//  NominalAttribute.h
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/29/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef NominalAttribute_h
#define NominalAttribute_h

#include "Attribute.h"
#include "string.h"
#include <vector>
#include <cstdio>

class NominalAttribute : public Attribute {
    
    friend ostream &operator<<( ostream &out, const NominalAttribute &na );
    
private:
    
    vector<string> domain; // example: types of tires
    
public:
    
    NominalAttribute();
    
    NominalAttribute( string name ) : Attribute(name) { }; // tire
    
    void addValue( string value ) throw ( logic_error ); // push type of tire onto domain 
    
    int getSize() const;
    
    string getValue( int index ) throw ( logic_error );
    
    virtual int getIndex( string value ) throw ( logic_error );
    
    virtual void print() const;
    
}; // NominalAttribute class


#endif /* NominalAttribute_h */
