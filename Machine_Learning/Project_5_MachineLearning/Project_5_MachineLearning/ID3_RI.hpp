//
//  ID3.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef ID3_hpp
#define ID3_hpp

#include <cstdio>
#include "Performance.hpp"
#include "Example.hpp"
#include "DataSet.hpp"
#include <time.h>
#include "Classifier.hpp"
#include <stdexcept>
#include <cstdlib>

// need to implment destructor(s)

class Node
{
public:
    Node() {label, attribute = -1;};
    
    vector< Node * > children;
    int label;
    int attribute;
    
    
    
};

class ID3_RI : public Classifier
{
    
private:
    Node * root;
    Attributes attribute;
    
    int _classify(Node * node, const Example & myExample);

    void _train_rand1(Node * node, const DataSet & myDataSet );
    void _train_rand2(Node * node, const DataSet & myDataSet );
    void _train_orig(Node * node, const DataSet & myDataSet );
    
    int k; // NEW for P5 - # in subset of random non-zero gain attirbutes to chose from all attributes
    
public:
    ID3_RI(int myK = 3) {root = new Node(); k = myK;};
    virtual int classify(const Example & myExample);
    //    virtual Performance classify(const DataSet &);
    virtual void train(const DataSet &);  
    
    //    void creatNewNode(
    
};



#endif /* ID3_hpp */
