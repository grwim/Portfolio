//
//  Example.h
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/27/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Example_h
#define Example_h

#include <vector>
#include <iostream>
#include <cstdio>

// Stores the attribute values of an example. Numeric values are stored as is. Nominal values are stored as doubles and are indices of the value in the attributes structure.
class Example : public std::vector<double>
{
    
    friend std::ostream &operator<<( std::ostream &out, const Example &e );
    
public:
    
    Example( int n ); // size of Example
    
}; // Example class

#endif /* Example_h */
