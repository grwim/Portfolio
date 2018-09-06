//
//  NaiveBayes.hpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef NaiveBayes_hpp
#define NaiveBayes_hpp

#include <cstdio>
#include "Classifier.h"
#include <vector>

class NaiveBayes : public Classifier
{
private:
//    double mean( vector<double> );
//    double stdev( vector<double> );
//    
//    double calcGausProb(double x, double mean, double stdev);
    
    void summarize(const DataSet & set);
    double getProbability(Example myExample,  vector<vector<double> >);
    vector<vector<double> > probabilites_class0;
    vector<vector<double> > probabilites_class1;

    
public:
    NaiveBayes();
    virtual void train(const DataSet & trainSet);
    vector<int> predict(const DataSet & testSet);
    

    
    // summarize?
    
    // calculateClassProbabilties?
    
};

#endif /* NaiveBayes_hpp */
