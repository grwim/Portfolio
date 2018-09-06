//
//  ID3.cpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//


#include "ID3_RI.hpp"

int ID3_RI::_classify(Node * node, const Example & myExample)
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

int ID3_RI::classify(const Example & myExample)
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
//
//void ID3_RI::_train_rand1(Node * node, const DataSet & ds) // if cant reach k, sets class labels of childen to majoirt class label of parent
//{
//    
//    // CALCULATE SUM OF WEIGHTS
//    
//    if (ds.entropy() == 0) // if dataset is homogonous
//    {
//        // create Leaf node
//        //        node = new Node();
//        node->label = ds.getExamples()[0][ds.getAttributes().getClassIndex()];
//        
//        //        cout << "LEAF NODE with label: " << ds.getExamples()[0][ds.getAttributes().getClassIndex()] << " created" << endl;
//    }
//    else
//    {
//        Attributes copy_Original = ds.getAttributes();
//        
//        srand ( time(NULL) );
//        
//        bool end = false;
//        vector<int> randSubSet; 
//        
//        while (randSubSet.size() < k && !end) {
//
//       int randIndex = rand() % (copy_Original.getSize() - 1); // - 1 so as to ignore class label
//            
//            if (ds.gain(randIndex) != 0) {
//                randSubSet.push_back(randIndex);
//                copy_Original.del(randIndex);
//            }
//            
//            if (copy_Original.getSize() == 0) { // ran out of attributes to populate k
//                end = true;
//            }
//            
//            // CHECK FOR k > K!!! --> if this happens, then set class label of children to majority class
//            
//        }
//        
//        if ( end ) { // ran out of attributes to populate k, so set class label of children to majority class 
//            
//            node->attribute = ds.getBestSplitAtt(randSubSet);
//            vector<DataSet> splits = ds.splitOnAtt(node->attribute);
//            
//            for (int i = 0; i < splits.size(); i++) {
//                //            CHECK THIS
//                vector<int> classCount(dynamic_cast<NominalAttribute *>(ds.getAttributes().getClassAttribute())->getSize()); // vector to keep count of class value occurences
//                
//                for (int j = 0; j < ds.getExamples().size(); j++) { // count class value occurnces
//                    classCount[ ds.getExamples()[j][ds.getAttributes().getClassIndex()]] += 1;
//                }
//                
//                int majorityClassLabel = 0;
//                int majorityCount = classCount[0] /* / sumWeights  */  ;
//
//                // find majority class label                                                 // normalize here by dividing by sum of weights??
//                for (int j = 1; j < classCount.size(); j++) { // count class value occurnces
//                    int tempCount = classCount[j];
//                    
//                    if (tempCount > majorityCount)
//                    {
//                        majorityClassLabel = j;
//                    }
//                }
//                //                cout << "EMPTY INTERNAL NODE- majorityClassLabel of parent is: " << majorityClassLabel << endl;
//                node->children[i]->label = majorityClassLabel;
//            }
//            
//        }
//        else // k was reached
//        {
//            
//            node->attribute = ds.getBestSplitAtt(randSubSet);
//            vector<DataSet> splits = ds.splitOnAtt(node->attribute);
//            
//            for (int i = 0; i < splits.size(); i++) {
//                
//                node->children.push_back(new Node());
//                
//                if (splits[i].getExamples().size() == 0) // set the label to the majority class of the data set in the parent node for a child with an empty data set.
//                {
//                    vector<int> classCount(dynamic_cast<NominalAttribute *>(ds.getAttributes().getClassAttribute())->getSize()); // vector to keep count of class value occurences
//                    
//                    for (int j = 0; j < ds.getExamples().size(); j++) { // count class value occurnces
//                        classCount[ ds.getExamples()[j][ds.getAttributes().getClassIndex()]] += 1;
//                    }
//                    
//                    int majorityClassLabel = 0;
//                    int majorityCount = classCount[0] /* / sumWeights  */  ;
//                    
//                    // find majority class label                                                 // normalize here by dividing by sum of weights??
//                    for (int j = 1; j < classCount.size(); j++) { // count class value occurnces
//                        int tempCount = classCount[j];
//                        
//                        if (tempCount > majorityCount)
//                        {
//                            majorityClassLabel = j;
//                        }
//                    }
//                    //                cout << "EMPTY INTERNAL NODE- majorityClassLabel of parent is: " << majorityClassLabel << endl;
//                    node->children[i]->label = majorityClassLabel;
//                    
//                }
//                else // know I don't have leaf node
//                {
//                    _train_rand1(node->children[i], splits[i]);
//                }
//                
//            }
//        }
//    }
//}
//

void ID3_RI::_train_rand2(Node * node, const DataSet & ds ) // if cant reach k, continues deterministcly
{
    // CALCULATE SUM OF WEIGHTS
    
    if (ds.entropy() == 0) // if dataset is homogonous
    {
        // create Leaf node
        //        node = new Node();
        node->label = ds.getExamples()[0][ds.getAttributes().getClassIndex()];
        
        //        cout << "LEAF NODE with label: " << ds.getExamples()[0][ds.getAttributes().getClassIndex()] << " created" << endl;
    }
    else
    {
        Attributes copy_Original = ds.getAttributes();
        
        srand ( time(NULL) );
        
        bool end = false;
        vector<int> randSubSet;
        
        while (randSubSet.size() < k && !end) {
            
            int randIndex = rand() % (copy_Original.getSize());
            
            if (randIndex != 0) { // - 1 so as to ignore class label
                randIndex -= 1;
            }
            
            if (ds.gain(randIndex) != 0) {
                randSubSet.push_back(randIndex);
                copy_Original.del(randIndex);
            }
            else
            {
                copy_Original.del(randIndex);
            }
            
            if (copy_Original.getSize() == 0) { // ran out of attributes to populate k
                end = true;
            }
//            cout << "here: " << randSubSet.size() << endl;
            // CHECK FOR k > K!!! --> if this happens, then set class label of children to majority class
            
        }
        
        
        if ( end ) { // ran out of attributes to populate k, so continue deterministicly
            
            
            // NEED TO USE OLD TRAIN FUNCTION?? (MAKE CALL)
            // yeah, just make call to original train that doesnt have random attribute selection/reduction
            
            // test against other train, and original ID3
            
            _train_orig(node, ds);
            
        }
        else // k was reached
        {
            
            node->attribute = ds.getBestSplitAtt(randSubSet);
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
                    int majorityCount = classCount[0] /* / sumWeights  */  ;
                    
                    // find majority class label                                                 // normalize here by dividing by sum of weights??
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
                    _train_rand2(node->children[i], splits[i]);
                }
                
            }
        }
    }

}

void ID3_RI::_train_orig(Node * node, const DataSet & ds )
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
                _train_orig(node->children[i], splits[i]);
            }
            
        }
    }

}

void ID3_RI::train(const DataSet & ds)
{
    _train_rand2(root, ds);
}


















