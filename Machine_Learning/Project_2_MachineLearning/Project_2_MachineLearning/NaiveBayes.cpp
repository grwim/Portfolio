//
//  NaiveBayes.cpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "NaiveBayes.h"

//NaiveBayes::NaiveBayes()
//{
//    
//}


NaiveBayes::NaiveBayes()
{
    
}


double NaiveBayes::getProbability(Example myExample, vector<vector<double> > probabilitiesTable) // returns probability
{
    double probability = 1.0;
    
    
    // for each atttribute in myExample, look up probability for value, multiply by eachother
    
    for (int i = 0; i < myExample.size(); i++) {
        
        probability *= probabilitiesTable[i][myExample[i]];
        
    }
    
    
    
    return probability;
}


vector<int> NaiveBayes::predict(const DataSet & testSet)
{
    vector<int> predictions;
    
    //  for every example in Dataset, getProbability with set0 and set1
    //  based on which porability is higher, return
    
    
    for (int i = 0; i < testSet.getExamples().size(); i++) {
        
        double prob0 = getProbability(testSet.getExamples()[i], probabilites_class0);
        
        double prob1 = getProbability(testSet.getExamples()[i], probabilites_class1);
        
        if (prob0 > prob1) {
            predictions.push_back(0);
        }
        else {
            predictions.push_back(1);
        }

    }
    
    return predictions;
}



//string name = dynamic_cast<NominalAttribute *>(tts.getTrainingSet().getAttributes().get(0))->getValue(0);
//
//cout << "INDEX is: " << dynamic_cast<NominalAttribute *>(tts.getTrainingSet().getAttributes().get(0))->getIndex(name) << endl;


void NaiveBayes::train(const DataSet & trainSet)
{
    summarize(trainSet);
}


void NaiveBayes::summarize(const DataSet & trainSet)
{
    
    
//    need to split list based on class value
//    then perform two summarize(), one  on each split
    
    
    DataSet set0(trainSet.getAttributes());
    DataSet set1(trainSet.getAttributes());
//    dynamic_cast<NominalAttribute *>(trainSet.getAttributes().getClassAttribute())->getValue(1)
    
//    cout << "Value 0: " << trainSet.getExamples()[0][trainSet.getAttributes().getClassIndex()] << endl;
    
    for (int i = 0; i < trainSet.getExamples().size() ; i++) {
        
        if (0 == trainSet.getExamples()[i][trainSet.getAttributes().getClassIndex()] ) {
            set0.add(trainSet.getExamples()[i]);
//            cout << "got 0 " << endl;
        }
        else if (1 == trainSet.getExamples()[i][trainSet.getAttributes().getClassIndex()] ) {
            set1.add(trainSet.getExamples()[i]);
//            cout << "got 1 " << endl;

        }
    }


    vector<double> list;
    
    unsigned long totalNumber0 = set0.getExamples().size();
    
    for (int i = 0 ; i < set0.getAttributes().getSize(); i++) { // for each attribute
        
        
        
        int attSize = dynamic_cast<NominalAttribute *>(set0.getAttributes().get(i))->getSize();
        //        cout << "size: " << attSize << " ";
        
        for (int j = 0; j < attSize; j++) { // for each possible value of attribute j
            
            int count = 0;
            string name = dynamic_cast<NominalAttribute *>(trainSet.getAttributes().get(i))->getValue(j);
            double indexVal = dynamic_cast<NominalAttribute *>(trainSet.getAttributes().get(i))->getIndex(name);
            
            //       cout << indexVal << " ";
            
            for (int z = 0; z < set0.getExamples().size(); z++) { // for all examples in train set
                //                cout << trainSet.getExamples()[z][i] << " ";
                if (set0.getExamples()[z][i] == indexVal)
                {
                    //                    cout << "GOT HERE" << endl;
                    count += 1;
                }
  
            }
            //            cout << endl;
            
            double prob = (count + 1) / static_cast<double>(totalNumber0);
            
//            cout << prob << " ";
            
            list.push_back(prob);  // USING +1 SMOOTHING HERE
            
        }
        
//        cout << endl;

        probabilites_class0.push_back(list);
        
        //        probabilites.push_back(list);
        
        list.clear();
        
    }
    
    
    unsigned long totalNumber1 = set1.getExamples().size();
    
    for (int i = 0 ; i < set1.getAttributes().getSize(); i++) { // for each attribute
        
        list.clear();
        
        int attSize = dynamic_cast<NominalAttribute *>(set1.getAttributes().get(i))->getSize();
//        cout << "size: " << attSize << " ";
        
        for (int j = 0; j < attSize; j++) { // for each possible value of attribute j
            
            int count = 0;
            string name = dynamic_cast<NominalAttribute *>(set1.getAttributes().get(i))->getValue(j);
            double indexVal = dynamic_cast<NominalAttribute *>(set1.getAttributes().get(i))->getIndex(name);
            
//       cout << indexVal << " ";
            
            for (int z = 0; z < set1.getExamples().size(); z++) { // for all examples in train set
//                cout << trainSet.getExamples()[z][i] << " ";
                if (set1.getExamples()[z][i] == indexVal)
                {
//                    cout << "GOT HERE" << endl;
                    count += 1;
                }
         
            }
//            cout << endl;
            
            
            double prob = (count + 1) / static_cast<double>(totalNumber1);
            
//            cout << prob << " ";
            
            list.push_back(prob);  // USING +1 SMOOTHING HERE
            
        }
//        cout << endl;
        
        probabilites_class1.push_back(list);
        
//        probabilites.push_back(list);
        
    }
}