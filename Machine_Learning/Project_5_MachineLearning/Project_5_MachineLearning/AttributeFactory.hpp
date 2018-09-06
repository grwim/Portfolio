//
//  AttributeFactory.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.

#ifndef AttributeFactory_hpp
#define AttributeFactory_hpp

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Attribute.hpp"
#include "NominalAttribute.hpp"
#include "NumericAttribute.hpp"
#include <stdexcept>

class AttributeFactory {
    
public:
    
    static Attribute * make( const vector<string> &data ) throw ( logic_error );
    
}; // AttributeFactory class

#endif /* AttributeFactory_hpp */

