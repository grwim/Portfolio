//
//  NominalAttribute.cpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.


#include "NominalAttribute.hpp"

NominalAttribute::NominalAttribute()
{
    
}

void NominalAttribute::addValue( string value ) throw ( logic_error ) // Adds a new nominal value to the domain of this nominal attribute.
{
    try {
        
        domain.push_back(value);
        
    } catch (string vectorPushError) {
        
        throw vectorPushError;
        
    }
}

int NominalAttribute::getSize() const
{
    
    //Gets the size of this attribute's domain.
    
    return static_cast<int>(domain.size());
    
}

string NominalAttribute::getValue( int index ) throw ( logic_error )
{
    
    //Returns the value of this nominal attribute at the specified index.
    
    return domain.at(index);
}

int NominalAttribute::getIndex( string value ) throw ( logic_error )
{
    
    int output = -1;
    string error = "Error encountered in: NominalAttribute::getIndex ";
    
    for (unsigned int i = 0; i < domain.size(); i++ )
    {
        if (strcmp(value.c_str(), domain[i].c_str()) == 0)
        {
            output = i;
        }
    }
    
    if (output == -1) {
        throw error;
    }
    
    return output;
    
}

ostream &operator<<( ostream &out, const NominalAttribute &na )
{
    na.print();
    return out;
}

void NominalAttribute::print( ) const
{
    cout << getName();
    
    for (unsigned int i = 0; i < domain.size(); i++ )
    {
        cout << " " << domain[i];
    }
    
}
