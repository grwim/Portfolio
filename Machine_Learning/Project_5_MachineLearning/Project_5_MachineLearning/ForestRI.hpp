//
//  Forest-RI.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/25/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef ForestRI_hpp
#define ForestRI_hpp

#include <stdio.h>
#include <vector>
#include "ID3_RI.hpp"
#include "Classifier.hpp"
#include "Performance.hpp"
#include "DataSet.hpp"
#include <map>
#include <stdexcept>

class ForestRI : public Classifier
{
private:
    
    // weights
    vector<ID3_RI> forest;
    
    int b; // size of forest
    int k; // # of non-zero gain attributes available to each split choice
    
public:
//    ForestRI();
    ForestRI(int myB, int myK) {b = myB; k = myK;};

    void setOptions(vector<string> & options );
    
    virtual int classify(const Example &);
    virtual void train(const DataSet &);
    virtual Performance classify(const DataSet &);
    virtual void clear() { forest.clear(); };
    
};




#endif /* Forest_RI_hpp */
