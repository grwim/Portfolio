//
//  BP.hpp
//  Project_4_MachineLearning
//
//  Created by Konrad Rauscher on 4/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef BP_hpp
#define BP_hpp

#include <cstdio>
#include "Performance.hpp"
#include "Example.hpp"
#include "DataSet.hpp"
#include "Classifier.hpp"
#include <time.h>
#include <math.h>
#include "string.h"

// calculate forward, then propogate back, tweaking V and W

// need to rolve nuancies in initializiation... J, I, K etc...

class Node
{
    Node ();
    
    // how to store connections, values, etc.
    
};

class BP : public Classifier
{
private:
    vector < vector<double> > v; // output layer weights
    vector < vector<double> > w; // hiddern layer weights
    
    double learnRate;
    int numInternNodes; // inside algorithm, nimInternNodes + 1 for bias term
    double minError;
    
public:
    BP();
    
    void setOptions(vector<string> & options );
    void setLearnRate(double myLearnRate) { learnRate = myLearnRate; };
    void setNumInternNodes(int myNum) { numInternNodes = myNum; }
    void setMinError( double myMinError) {minError = myMinError; }
    
    double getLearnRate() { return learnRate; }
    int getNumInternNodes() { return numInternNodes; }
    double getMinError() { return minError; }
    
    virtual int classify(const Example &);
    virtual Performance classify( const DataSet &);
    virtual void train(const DataSet &);
 
};


#endif /* BP_hpp */
