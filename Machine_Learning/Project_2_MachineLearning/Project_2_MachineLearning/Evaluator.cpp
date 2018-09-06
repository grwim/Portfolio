//
//  Evaluator.cpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/25/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Evaluator.h"

Evaluator::Evaluator()
{
    
}

Evaluator::Evaluator(int num_folds)
{
   
    if (num_folds == -1) {
        folds = 10;
    }
    else
    {
        folds = num_folds;
    }

}

void Evaluator::setFolds(int num_folds)
{
    folds = num_folds;
}

double Evaluator::standardDev(vector<double> probabilities ) // of probabilities
{
    double stdDev;
    
    /*
     Work out the Mean (the simple average of the numbers)
     Then for each number: subtract the Mean and square the result.
     Then work out the mean of those squared differences.
     Take the square root of that and we are done
     */
    
    double mean;
    double sum;
    
    for (int i = 0; i < probabilities.size(); i++) {
//        cout << probabilities[i] << " ";
        
        sum += probabilities[i];
    }
//    cout << endl;
   
    mean = sum / probabilities.size();
//    cout << "mean " << mean << endl;
    
    vector<double> differences;
    
    for (int i = 0; i < probabilities.size(); i++) {
        differences.push_back((probabilities[i] - mean)*(probabilities[i] - mean));
    }
    
    double meanSq;
    for ( int i = 0; i < differences.size(); i++) {
        meanSq += differences[i];
    }
    
    meanSq /= differences.size();
    
    stdDev = sqrt(meanSq);
    
    return stdDev;
}


vector<DataSet> Evaluator::partition(DataSet dSet)   // debug partition
{
    vector<DataSet> partitions;
    srand ( time(NULL) );
    // using folds
    
    unsigned long partitionSize = (dSet.getExamples().size() / folds);
    
    
    for (int i = 0; i < folds; i++) {                       // create partitions
        
        DataSet * partition = new DataSet(dSet.getAttributes());
        while (partition->getExamples().size() < partitionSize) {
            
            int randIndex = rand() % dSet.getExamples().size();
            partition->add(dSet.getExamples()[randIndex]);
            
        }
        partitions.push_back(*partition);
        
    }

    return partitions;
}


Performance Evaluator::evaluate(Classifier & classifier, DataSet dSet)
{
 
    vector<double> accuracies;
 
    Performance performance;
    
    Classifier * classPtr = &classifier;
    
    vector<DataSet> partitions = partition(dSet);
    
//    for (int i = 0; i < partitions.size(); i++) {
//        
//        cout << "example " << i << " ";
//        for (int j = 0; j < partitions[i].getExamples()[i].size(); j++) {
//            cout << partitions[i].getExamples()[i][j] << " ";
//        }
//        cout << endl;
//        
//    }
    
//    cout << "size: " << dSet.getExamples().size() << endl;
    
    DataSet train(dSet.getAttributes());
    DataSet test(dSet.getAttributes());
    
    // FOR I < folds, I++
    // get list of predictions, compare against actual, calculate accuracy
    
    // have to partition first.  
    
    if (typeid(*classPtr) == typeid(KNN))
    {
        
        vector<int> predictions;
        
        int countCorrect = 0;
        
        for (int i = 0; i < folds; i++) { // do for k- number of folds
            
            for (int j = 0; j < folds; j++)  {  // insert partitions into appropriate DataSets
                if (i != j) {
                    train.add(partitions[j]);
                }
                else {
                    test.add(partitions[j]);
                }
            }
            
//            for (int i = 0; i < train.getExamples().size(); i++) {
//                
//                cout << "example " << i << " ";
//                for (int j = 0; j < train.getExamples()[i].size(); j++) {
//                    cout << train.getExamples()[i][j] << " ";
//                }
//                cout << endl;
//                
//            }
            
            // do calculation here for accuracy
            
            dynamic_cast<KNN *>(classPtr)->train(train);
            predictions = dynamic_cast<KNN *>(classPtr)->predict(test);
            
            countCorrect = 0;
            
//            cout << "size of test is " << test.getExamples().size() << endl;
//            cout << "size of train is " << train.getExamples().size() << endl;
            
            for (int i = 0; i < test.getExamples().size(); i++) { // get count of # correct
                
//                cout << "prediction is " << predictions[i] << " and actual is " << test.getExamples()[i][test.getAttributes().getClassIndex()] << endl;
                
                if (predictions[i] == test.getExamples()[i][test.getAttributes().getClassIndex()]) {
                    countCorrect++;
                }
            }
//            cout << "countCorrect: " << countCorrect << endl;
//            cout << "Accuracy: " << countCorrect / static_cast<double>(test.getExamples().size()) << endl;
            accuracies.push_back(countCorrect / static_cast<double>(test.getExamples().size()));   // push back accuracy
            
//            cout << "Evaluation  " << i << " with size " << train.getExamples().size() << " " << test.getExamples().size() << endl;
            
            
            train.clearExamples();
            test.clearExamples();
            
            
        }

    }
    else if(typeid(*classPtr) == typeid(NaiveBayes))
    {
        
        vector<int> predictions;
        
        int countCorrect = 0;
        
        for (int i = 0; i < folds; i++) { // do for k- number of folds
            
            for (int j = 0; j < folds; j++)  {  // insert partitions into appropriate DataSets
                if (i != j) {
                    train.add(partitions[j]);
                }
                else {
                    test.add(partitions[j]);
                }
            }
            
            //            for (int i = 0; i < train.getExamples().size(); i++) {
            //
            //                cout << "example " << i << " ";
            //                for (int j = 0; j < train.getExamples()[i].size(); j++) {
            //                    cout << train.getExamples()[i][j] << " ";
            //                }
            //                cout << endl;
            //
            //            }
             
            // do calculation here for accuracy
            
            dynamic_cast<NaiveBayes *>(classPtr)->train(train);
            predictions = dynamic_cast<NaiveBayes *>(classPtr)->predict(test);
            
            countCorrect = 0;
            
            //            cout << "size of test is " << test.getExamples().size() << endl;
            //            cout << "size of train is " << train.getExamples().size() << endl;
            
            for (int i = 0; i < test.getExamples().size(); i++) { // get count of # correct
                
                //                cout << "prediction is " << predictions[i] << " and actual is " << test.getExamples()[i][test.getAttributes().getClassIndex()] << endl;
                
                if (predictions[i] == test.getExamples()[i][test.getAttributes().getClassIndex()]) {    // WHY IS COUNT CORRECT INCREASING BY FACTOR OF 9???
                    countCorrect++;
                }
            }
//            cout << "countCorrect: " << countCorrect << endl;
//            cout << "Accuracy: " << countCorrect / static_cast<double>(test.getExamples().size()) << endl;
            accuracies.push_back(countCorrect / static_cast<double>(test.getExamples().size()));   // push back accuracy
            
            train.clearExamples();
            test.clearExamples();
        }
    }
    
    performance.addAccuracyList(accuracies);
    
//    cout << "Stand Dev: " << standardDev(accuracies) << endl;
    
    performance.set_stdDev(standardDev(accuracies)); // NEED TO CALC STAND DEV
    
    return performance;
    
}