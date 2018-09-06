//
//  Classifier.hpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Classifier_hpp
#define Classifier_hpp

#include <cstdio>
#include "Examples.h"
#include "Attributes.h"
#include "Performance.h"
#include "DataSet.h"

class Classifier{
public:
    Classifier() {};
    virtual void train(const DataSet &);
    virtual Performance classify(const DataSet &);
    virtual int classify(const Example &);
    
    //int folds = 10;
    
};



#endif /* Classifier_hpp */
