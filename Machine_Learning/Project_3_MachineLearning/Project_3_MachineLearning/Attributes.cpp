//
//  Attributes.cpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Attributes.hpp"

Attributes::Attributes()
{
    hasNumericAttributes = false;
    hasNominalAttributes = false;
}

Attributes::~Attributes()
{
    
    attributes.clear();
    
}

void Attributes::add( Attribute &attribute ) throw ( logic_error )
{
    
    try {
        
        Attribute * attPtr = &attribute;
        
        attributes.push_back(attPtr);
        
        if (typeid(*attPtr) == typeid(NominalAttribute))
        {
            //            cout << "current att is: ";
            //            dynamic_cast<NominalAttribute *>(attPtr)->print();
            //            cout << endl;
            //            if (attPtr->getName() == "make") {
            //                cout << endl << "Index of 'trek' is: " << dynamic_cast<NominalAttribute *>(attPtr)->getIndex("trek") << endl;   // doesnt like this line
            //            }
            
            hasNominalAttributes = true;
            
        }
        else if(typeid(*attPtr) == typeid(NumericAttribute))
        {
            //            cout << "current att is: ";
            //            dynamic_cast<NumericAttribute *>(attPtr)->print();
            //            cout << endl;
            
            hasNumericAttributes = true;
            
        }
        
        setClassIndex(static_cast<int>( attributes.size()) - 1);
        //cout << "attributes size is: " << attributes.size() << endl;
        
    } catch (string error) {
        
        throw error;
        
    }
    
    classIndex = (attributes.size()-1);
    
}

int Attributes::getClassIndex() const throw ( logic_error )
{
    return classIndex;
}

bool Attributes::getHasNominalAttributes() const
{
    
    return hasNominalAttributes;
}

bool Attributes::getHasNumericAttributes() const
{
    
    return hasNumericAttributes;
}

Attribute * Attributes::get( int i ) const throw ( logic_error )
{
    if (i > attributes.size() || i < 0) {
        cout << "ERROR in Attributes::get: out of range" << endl;
    }
    
    
    return attributes[i];
    
}

Attribute * Attributes::getClassAttribute() const throw ( logic_error )
{
    return attributes[classIndex];
}

int Attributes::getIndex( string name ) const throw ( logic_error )
{
    
    int index = -1;
    for (unsigned int i = 0; i < attributes.size(); i++) {
        if (name == attributes[i]->getName()) {
            
            index = i;
            
        }
    }
    
    return index;
}

int Attributes::getSize() const
{
    
    return static_cast<int>(attributes.size());
}

void Attributes::parse( const vector<string> &data ) throw ( logic_error )
{
    
    AttributeFactory attFact;
    
    Attribute * att = attFact.make( data );
    
    add(*att);
    
}

void Attributes::setClassIndex( int myClassIndex ) throw ( logic_error )
{
    classIndex = myClassIndex;
}

ostream &operator<<( ostream &out, const Attributes &a )
{
    out << "@attribute ";
    for (unsigned int i = 0; i < a.attributes.size(); i++) {
        
        a.get(i)->print();
        
        out << " ";
    }
    out << endl;
    
    
    return out;
}

void Attributes::print() const
{
    
    for (unsigned int i = 0; i < attributes.size(); i++) {
        cout << "@attribute ";
        get(i)->print();
        
        cout << endl;
    }
    cout << endl;
    
}