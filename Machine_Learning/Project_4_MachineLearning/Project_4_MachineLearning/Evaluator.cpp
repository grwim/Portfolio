//
//  Evaluator.cpp
//  Project_4_MachineLearning
//
//  Created by Konrad Rauscher on 4/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Evaluator.hpp"

Evaluator::Evaluator()
{
    kFold_asCurrDefault = true;
}

Evaluator::Evaluator(int num_folds)
{
    kFold_asCurrDefault = true;
    
    if (num_folds == -1) {
        folds = 10;
    }
    else
    {
        folds = num_folds;
    }
}

Evaluator::Evaluator(vector<string> evalOptions)
{
    setOptions(evalOptions);
    
}

void Evaluator::setOptions(vector<string> evalOptions)
{
    
    if (evalOptions[0] == "x")
    {
        folds = atoi(evalOptions[1].c_str());
    }
    else if (evalOptions[0] == "p")
    {
        kFold_asCurrDefault = false;
        proportion = stod(evalOptions[1].c_str());
    }
    else
    {
        cout << "ERROR in Evaluator::setOptions" << endl;
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

Performance Evaluator::evaluate(Classifier & classifier, DataSet trainSet)
{
    //    cout << "here: " << kFold_asCurrDefault << endl;
    if (kFold_asCurrDefault) {
        return _evaluate_kFold(classifier, trainSet);
    }
    
    // (implicit) else:
    return _evaluate_holdOut(classifier, trainSet);
    
}

Performance Evaluator::evaluate(Classifier & classifier, DataSet train, DataSet test) // NEW for project 3
{
    Performance performance;
    
    Classifier * classifierPtr = &classifier;
    
    vector<int> predictions;
    
    vector<double> accuracies;
    
    //    proportion specifies proportion used as training set, train at index 0
    
    // now. train with train, and test with test, return performance
    
    
    classifierPtr->train(train); // train with train
    
    // predict on test
    for (int i = 0; i < test.getExamples().size(); i++) {
        predictions.push_back(classifierPtr->classify(test.getExamples()[i]));
        //            cout << "prediction: " << classifierPtr->classify(test.getExamples()[i]) << endl;
    }
    
    int countCorrect = 0;
    
    //                cout << "size of test is " << test.getExamples().size() << endl;
    //                cout << "size of train is " << train.getExamples().size() << endl;
    
    for (int i = 0; i < test.getExamples().size(); i++) { // get count of # correct
        
        //                cout << "prediction is " << predictions[i] << " and actual is " << test.getExamples()[i][test.getAttributes().getClassIndex()] << endl;
        
        if (predictions[i] == test.getExamples()[i][test.getAttributes().getClassIndex()]) {
            countCorrect++;
            //            cout << " correct " << endl;
        }
    }
    //            cout << "countCorrect: " << countCorrect << endl;
    //            cout << "Accuracy: " << countCorrect / static_cast<double>(test.getExamples().size()) << endl;
    accuracies.push_back(countCorrect / static_cast<double>(test.getExamples().size()));   // push back accuracy
    
    performance.setAccuracyList(accuracies);
    
    //            cout << "Evaluation  " << i << " with size " << train.getExamples().size() << " " << test.getExamples().size() << endl;
    
    return performance;
}

Performance Evaluator::_evaluate_holdOut(Classifier & classifier, DataSet dSet) // NEW for project 3
{
    //    cout << "got here " << endl;
    Performance performance;
    
    Classifier * classifierPtr = &classifier;
    
    vector<int> predictions;
    
    vector<double> accuracies;
    
    //    proportion specifies proportion used as training set, train at index 0
    vector<DataSet> trainAndTest = dSet.splitByProportion(proportion);
    
    // now. train with train, and test with test, return performance
    
    
    classifierPtr->train(trainAndTest[0]); // train with train
    
    // predict on test
    for (int i = 0; i < trainAndTest[1].getExamples().size(); i++) {
        predictions.push_back(classifierPtr->classify(trainAndTest[1].getExamples()[i]));
        //            cout << "prediction: " << classifierPtr->classify(test.getExamples()[i]) << endl;
    }
    
    int countCorrect = 0;
    
    //            cout << "size of test is " << test.getExamples().size() << endl;
    //            cout << "size of train is " << train.getExamples().size() << endl;
    
    for (int i = 0; i < trainAndTest[1].getExamples().size(); i++) { // get count of # correct
        
        //                cout << "prediction is " << predictions[i] << " and actual is " << test.getExamples()[i][test.getAttributes().getClassIndex()] << endl;
        
        if (predictions[i] == trainAndTest[1].getExamples()[i][trainAndTest[1].getAttributes().getClassIndex()]) {
            countCorrect++;
            //            cout << " correct " << endl;
        }
    }
    //            cout << "countCorrect: " << countCorrect << endl;
    //    cout << "Accuracy: " << countCorrect / static_cast<double>(trainAndTest[1].getExamples().size()) << endl;
    accuracies.push_back(countCorrect / static_cast<double>(trainAndTest[1].getExamples().size()));   // push back accuracy
    
    performance.setAccuracyList(accuracies);
    
    //            cout << "Evaluation  " << i << " with size " << train.getExamples().size() << " " << test.getExamples().size() << endl;
    
    return performance;
}

Performance Evaluator::_evaluate_kFold(Classifier & classifier, DataSet dSet)
{
    
    if (folds > dSet.getExamples().size()) {
        string Error = " ERROR in _evaluate_kFold: too few examples in data set to perform cross fold";
        
        throw Error;
    }
    
    
    vector<double> accuracies;
    
    Performance performance;
    
    Classifier * classifierPtr = &classifier;
    
    vector<DataSet> partitions = dSet.partition(folds);
    
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
        
//                    for (int i = 0; i < train.getExamples().size(); i++) {
//        
//                        cout << "example " << i << " ";
//                        for (int j = 0; j < train.getExamples()[i].size(); j++) {
//                            cout << train.getExamples()[i][j] << " ";
//                        }
//                        cout << endl;
//        
//                    }
        
        // do calculation here for accuracy
//        cout << "fold " << i << ", with train of size: " << train.getExamples().size() <<  endl;
        
        classifierPtr->train(train);
        
        for (int i = 0; i < test.getExamples().size(); i++) {
            predictions.push_back(classifierPtr->classify(test.getExamples()[i]));
            //            cout << "prediction: " << classifierPtr->classify(test.getExamples()[i]) << endl;
        }
        
        countCorrect = 0;
        
        //            cout << "size of test is " << test.getExamples().size() << endl;
        //            cout << "size of train is " << train.getExamples().size() << endl;
        
        for (int i = 0; i < test.getExamples().size(); i++) { // get count of # correct
            
//            cout << "prediction is " << predictions[i] << " and actual is " << test.getExamples()[i][test.getAttributes().getClassIndex()] << endl;
            
            if (predictions[i] == test.getExamples()[i][test.getAttributes().getClassIndex()]) {
                countCorrect++;
//                                cout << " correct " << endl;
            }
        }
//        cout << "countCorrect: " << countCorrect << endl;
//        cout << "Accuracy: " << countCorrect / static_cast<double>(test.getExamples().size()) << endl;
        accuracies.push_back(countCorrect / static_cast<double>(test.getExamples().size()));   // push back accuracy
        
        //            cout << "Evaluation  " << i << " with size " << train.getExamples().size() << " " << test.getExamples().size() << endl;
        
        
        train.clearExamples();
        test.clearExamples();
    }
    
    performance.setAccuracyList(accuracies);
    
    //    cout << "Stand Dev: " << standardDev(accuracies) << endl;
    
    performance.set_stdDev(standardDev(accuracies)); // NEED TO CALC STAND DEV
    
    return performance;
    
}
