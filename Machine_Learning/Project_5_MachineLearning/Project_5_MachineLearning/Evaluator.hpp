//
//  Evaluator.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Evaluator_hpp
#define Evaluator_hpp

#include <cstdio>
#include "Classifier.hpp"
#include <vector>
#include "Performance.hpp"
#include <cmath>
#include <stdexcept>


class Evaluator
{
private:
    int folds;
    double proportion;
    double standardDev(vector<double> probabilities);
    
    bool kFold_asCurrDefault;
    
public:
    
    Evaluator();
    Evaluator(int num_folds);
    Evaluator(vector<string> evalOptions);
    
    void setFolds(int num_folds);
    void setOptions(vector<string> evalOptions);  // NEW for project 3
    
    Performance evaluate(Classifier &, DataSet train);
    Performance evaluate(Classifier &, DataSet train, DataSet test);
    
    Performance _evaluate_kFold(Classifier & , DataSet );
    Performance _evaluate_holdOut(Classifier &, DataSet ); // NEW for project 3
};

#endif /* Evaluator_hpp */
