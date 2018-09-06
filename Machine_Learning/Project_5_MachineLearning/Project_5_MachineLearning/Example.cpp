//
//  Example.cpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Example.hpp"

// is a child of vector< double >

// Stores the attribute values of an example. Numeric values are stored as is. Nominal values are stored as doubles and are indices of the value in the attributes structure.
Example::Example( int n ) // sets size of vector
{
    this->resize(n);
}

std::ostream &operator<<( std::ostream &out, const Example &e )
{
    
    return out;
}

