//
//  AttributeFactory.hpp
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 2/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//


#ifndef AttributeFactory_h
#define AttributeFactory_h

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Attribute.h"
#include "NominalAttribute.h"
#include "NumericAttribute.h"

class AttributeFactory {
    
public:
    
    static Attribute * make( const vector<string> &data ) throw ( logic_error );
    
}; // AttributeFactory class

#endif /* AttributeFactory_h */
