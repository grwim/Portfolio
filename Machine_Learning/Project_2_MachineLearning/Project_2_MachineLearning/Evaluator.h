//
//  Evaluator.hpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/25/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Evaluator_h
#define Evaluator_h

#include <cstdio>
#include "Classifier.h"
#include <vector>
#include "Performance.h"
#include "KNN.h"
#include "NaiveBayes.h"
#include <time.h>
#include <cmath>


class Evaluator
{
private:
    int folds;
    
    vector<DataSet> partition(DataSet);
    
    double standardDev(vector<double> probabilities);
    
public:
    Evaluator();
    Evaluator(int num_folds);
    void setFolds(int num_folds);
    Performance evaluate(Classifier & , DataSet testSet);
};


#endif /* Evaluator_h */
