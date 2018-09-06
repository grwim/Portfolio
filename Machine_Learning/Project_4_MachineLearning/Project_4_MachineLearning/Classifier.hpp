//
//  Classifier.hpp
//  Project_4_MachineLearning
//
//  Created by Konrad Rauscher on 4/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Classifier_hpp
#define Classifier_hpp

#include <cstdio>
#include "Examples.hpp"
#include "Attributes.hpp"
#include "Performance.hpp"
#include "DataSet.hpp"

class Classifier{
public:
    Classifier() {};
    virtual void train(const DataSet &);
    virtual Performance classify(const DataSet &);
    virtual int classify(const Example &);
    
    //int folds = 10;
    
};

#endif /* Classifier_hpp */
