//
//  KNN.cpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "KNN.h"


KNN::KNN(int myK )
{
    if (myK == -1)
    {
        k = 3;
    }
    else
    {
        k = myK;
    }
}

int KNN::distance(const Example & myExample, const Example & otherExample)
{
    int d = 0;

        for (int j = 0; j < otherExample.size(); j++) {
            if (otherExample[j] != myExample[j])
            {
                d += 1;
            }
        }
    
    return d;
}

int KNN::classify(const Example & myExample) // calling distance, find nearest k neighbors, returns prediction of class value based on majortity of class values for nearest k neighbors
{
    
    double bestLabel = -1;
    int min_dist = INT_MAX;
    vector<int> distance_Smallest(k, 99999);
    vector<double> classValue_Smallest(k, -1.0);
    
    
    for (int i = 0; i < instances.getExamples().size(); i++) {
        
        int d = distance(myExample, instances.getExamples()[i]);
//        cout << "Distance is: " << d << endl;
//        
//        cout << "GOT HERE " << endl;
        if (d < min_dist)
        {
            
            min_dist = d;
            bestLabel = instances.getExamples()[i][instances.getAttributes().getClassIndex()];
            int largest_dist = distance_Smallest[0];
            int indexLargest_dist = 0;
            
//            cout << "classIndex: " << instances.getAttributes().getClassIndex() << endl;
//            cout << "bestLabel: " << bestLabel << endl;
            
            for (int i = 1; i < distance_Smallest.size(); i++) { // find largest small value
                if (largest_dist < distance_Smallest[i]) {
                    largest_dist = distance_Smallest[i];
                    indexLargest_dist = i;
                }
            }
            
            distance_Smallest[indexLargest_dist] = min_dist;
            classValue_Smallest[indexLargest_dist] = bestLabel;
            
            
        }
        
    }
    
    // predict class value based on majority of class values in classValue_Smallest
    
    int count0 = 0;
    int count1 = 0;
    
//    double value1 = instances.getExamples()[0][instances.getAttributes().getClassIndex()];
//    double value2
    // obtain both class values
    
    string val0 = dynamic_cast<NominalAttribute *>(instances.getAttributes().getClassAttribute())->getValue(0);
    int index0 = dynamic_cast<NominalAttribute *>(instances.getAttributes().getClassAttribute())->getIndex(val0);
    
    string val1 = dynamic_cast<NominalAttribute *>(instances.getAttributes().getClassAttribute())->getValue(1);
    int index1 = dynamic_cast<NominalAttribute *>(instances.getAttributes().getClassAttribute())->getIndex(val1);
    
//    cout << "IMPORTANT: " << classValue_Smallest[0] << endl;

    for (int i = 0; i < classValue_Smallest.size(); i++) {
        if (classValue_Smallest[i] == index0) {
            count0 += 1;
        }
        else if (classValue_Smallest[i] == index1)
        {
            count1 += 1;
        }
    }
    
    int prediction;
    
    if (count0 > count1) {
        prediction = index0;
    }
    else {
        prediction = index1;
    }
    
    return prediction;
    
}

void KNN::train(const DataSet & trainData) // pass Dataset, use to populate instances
{

    instances.add(trainData);
    
}

vector<int> KNN::predict(const DataSet & testSet) // pass testSet, for every element in test set, use vote, return list of predictions
{
    vector<int> predictions;
    
    for (int i =0; i < testSet.getExamples().size(); i++) {
        predictions.push_back(classify(testSet.getExamples()[i]));
    }
    
    return predictions;
}

//int KNN::getDistance(const Example & myExample, const Example & otherExample)
//{
//    return distance(myExample,otherExample);
//}
//
//int KNN::getVote(const Example & myExample)
//{
//    return vote(myExample);
//}
