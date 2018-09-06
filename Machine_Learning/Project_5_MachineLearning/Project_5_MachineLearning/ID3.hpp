//
//  ID3.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 5/1/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef ID3_hpp
#define ID3_hpp

#include <cstdio>
#include "Performance.hpp"
#include "Example.hpp"
#include "DataSet.hpp"
#include "Classifier.hpp"
#include <stdexcept>

// need to implment destructor(s)

class Node
{
public:
    Node() {label, attribute = -1;};
    
    vector< Node * > children;
    int label;
    int attribute;
    
};

class ID3 : public Classifier
{
    
private:
    Node * root;
    Attributes attribute;
    
    int _classify(Node * node, const Example & myExample);
    void _train(Node * node, const DataSet &);
    
public:
    ID3() {root = new Node(); };
    virtual int classify(const Example & myExample);
    //    virtual Performance classify(const DataSet &);
    virtual void train(const DataSet &);  // make derive function, have to declare as virtual
    
    //    void creatNewNode(
    
};



#endif /* ID3_hpp */
