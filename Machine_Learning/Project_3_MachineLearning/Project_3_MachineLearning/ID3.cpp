//
//  ID3.cpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "ID3.hpp"

int ID3::_classify(Node * node, const Example & myExample)
{
    if (!(node->children.empty())) {
//        cout << "label: " << node->label << " and attribute: " << node->attribute << endl;
        return _classify(node->children[myExample[node->attribute]], myExample);
    }
    else
    {
        return node->label;
    }
}

int ID3::classify(const Example & myExample)
{
    return _classify(root, myExample);
}

//Performance ID3::classify(const DataSet & ds)
//{
//    
//    
//    
//    
//    
//    Performance blah;
//    
//    return blah;
//}

void ID3::_train(Node * node, const DataSet & ds)
{
    if (ds.entropy() == 0) // if dataset is homogonous
    {
        // create Leaf node
//        node = new Node();
        node->label = ds.getExamples()[0][ds.getAttributes().getClassIndex()];

//        cout << "LEAF NODE with label: " << ds.getExamples()[0][ds.getAttributes().getClassIndex()] << " created" << endl;
    }
    else
    {
 
        node->attribute = ds.getBestSplitAtt();
//        cout << "ds.getBestSplitAtt(): " << ds.getBestSplitAtt() << endl;
        
        vector<DataSet> splits = ds.splitOnAtt(node->attribute);
        
        for (int i = 0; i < splits.size(); i++) {
            
            node->children.push_back(new Node());
            
            if (splits[i].getExamples().size() == 0) // set the label to the majority class of the data set in the parent node for a child with an empty data set.
            {
                vector<int> classCount(dynamic_cast<NominalAttribute *>(ds.getAttributes().getClassAttribute())->getSize()); // vector to keep count of class value occurences
                
                for (int j = 0; j < ds.getExamples().size(); j++) { // count class value occurnces
                    classCount[ ds.getExamples()[j][ds.getAttributes().getClassIndex()]] += 1;
                }
                
                int majorityClassLabel = 0;
                int majorityCount = classCount[0];
                
                // find majority class label
                for (int j = 1; j < classCount.size(); j++) { // count class value occurnces
                    int tempCount = classCount[j];
                    
                    if (tempCount > majorityCount)
                    {
                        majorityClassLabel = j;
                    }
                }
//                cout << "EMPTY INTERNAL NODE- majorityClassLabel of parent is: " << majorityClassLabel << endl;
                node->children[i]->label = majorityClassLabel;
                
            }
            else // know I don't have leaf node
            {
                _train(node->children[i], splits[i]);
            }
            
        }
    }
}

void ID3::train(const DataSet & ds)
{
    _train(root, ds);
}

















