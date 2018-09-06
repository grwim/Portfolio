//
//  Forest-RI.cpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/25/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "ForestRI.hpp"

//create n trees
//
//Create n random subsets with replacement
//
//train n trees
//
//determine rankings for n trees by training on data set (main) ?? need to determine ranking or not?
//poll from trees

int ForestRI::classify(const Example & myEX)
{
    
    vector<int> predictions;
    
    for (int i = 0; i < b; i++) {
        predictions.push_back(forest[i].classify(myEX));
    }
    
    int max = 0;
    int most_common = -1;
    map<int,int> m;
    vector<int>::iterator vi;
    for (vi = predictions.begin(); vi != predictions.end(); vi++) {
        m[*vi]++;
        if (m[*vi] > max) {
            max = m[*vi];
            most_common = *vi;
        }
    }
    
    return most_common;
}

void ForestRI::train(const DataSet & myDS)
{

    vector<DataSet> bootstrap = myDS.bootstrap(b); // NEED TO GET OUT OF BAG EXAMPLES (EXAMPLES NOT IN BOOTSRAP) TO USE AS TEST SETS ??
    
    for (int i = 0; i < b; i++) {
        ID3_RI newID3 = * new ID3_RI(k);
        forest.push_back(newID3);
    }
    
    // train n trees ... need to hold out random features
    
    for (int i = 0; i < b; i++) {
        forest[i].train(bootstrap[i]);
    }
}

Performance ForestRI::classify(const DataSet & myDS)
{
    Performance performance;
    
    vector<int> predictions;
    
    vector<double> accuracies;

    for (int i = 0; i < myDS.getExamples().size(); i++) {
        predictions.push_back(this->classify(myDS.getExamples()[i]));
        //                    cout << "prediction: " << this->classify(adjusted.getExamples()[i]) << endl;
    }
    
    int countCorrect = 0;
    
    //                cout << "size of test is " << test.getExamples().size() << endl;
    //    cout << "size of ds is " << ds.getExamples().size() << endl;
    
    for (int i = 0; i < myDS.getExamples().size(); i++) { // get count of # correct
        
        //                cout << "prediction is " << predictions[i] << " and actual is " << test.getExamples()[i][test.getAttributes().getClassIndex()] << endl;
        
        if (predictions[i] == myDS.getExamples()[i][myDS.getAttributes().getClassIndex()]) {
            countCorrect++;
            //            cout << " correct " << endl;
        }
    }
    //    cout << "countCorrect: " << countCorrect << endl;
    //    cout << "Accuracy: " << countCorrect / static_cast<double>(adjusted.getExamples().size()) << endl;
    accuracies.push_back(countCorrect / static_cast<double>(myDS.getExamples().size()));   // push back accuracy
    
    performance.setAccuracyList(accuracies);
    
    //            cout << "Evaluation  " << i << " with size " << train.getExamples().size() << " " << test.getExamples().size() << endl;
    return performance;
    
}

void ForestRI::setOptions(vector<string> & options )
{
    
    for (int i = 0; i < options.size(); i++) {
        if (options[i][0] == 'k') {
            i++;
            k = stof(options[i]);
        }
        else if (options[i][0] == 'b')
        {
            i++;
            b = stof(options[i]);
        }
    }
}




