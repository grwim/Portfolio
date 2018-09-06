//
//  KNN.hpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef KNN_hpp
#define KNN_hpp

#include <cstdio>
#include "Classifier.h"
#include "DataSet.h"
#include "Example.h"


// The implementation of k-NN should include the command-line switch -k to specify the value of k. Use 3 as the default.

class KNN : public Classifier
{
    
private:
    int distance(const Example & item, const Example & otherItem);

    int k;
    DataSet instances;
    
public:
    KNN() {};
    KNN(int myK );
    virtual int classify(const Example & myExample);
    virtual void train(const DataSet &);  // make derive function, have to declare as virutal
    vector<int> predict(const DataSet &); // should place predict in evaluator 
    
//    int getDistance(const Example &, const Example &);
//    int getVote(const Example & myExample);
    
};

#endif /* KNN_hpp */
