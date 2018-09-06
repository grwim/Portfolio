//
//  Attributes.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Attributes_hpp
#define Attributes_hpp

#include <vector>
#include <iostream>
#include <typeinfo>
#include "Attribute.hpp"
#include "AttributeFactory.hpp"
#include <cstdio>
#include <stdexcept>


class Attributes {
    
    friend ostream &operator<<( ostream &out, const Attributes &a );
    
private:
    
    vector<Attribute *> attributes;
    
    bool hasNumericAttributes;
    
    bool hasNominalAttributes;
    
    int classIndex; // = to last index in vector
    
public:
    
    Attributes();
    
    ~Attributes();
    
    void add( Attribute &attribute ) throw ( logic_error );
    
    int getClassIndex() const throw ( logic_error );
    
    bool getHasNominalAttributes() const;
    
    bool getHasNumericAttributes() const;
    
    Attribute * get( int i ) const throw ( logic_error );
    
    Attribute * getClassAttribute() const throw ( logic_error ); // returns attribute at classIndex
    
    void del( int i );
    
    int getIndex( string name ) const throw ( logic_error );
    
    int getSize() const;
    
    void parse( const vector<string> &data ) throw ( logic_error );
    
    void setClassIndex( int classIndex ) throw ( logic_error );
    
    void print () const;
    
}; // Attributes class

#endif /* Attributes_hpp */
