//
//  forestri-main.cpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 5/1/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "forestri-main.hpp"

int main(int argc, const char * argv[]) {

    string fileOpenError = "";
    vector<string> options;
    bool testSet = false;
    vector<string> evalOptions;
    
    string trainFilename;
    string testFilename;
    
    vector<string> RIOptions;
    
    try {
        
        for (int i = 1; i < argc; i++) {
            string s(argv[i]);
            options.push_back(s);
        }
        
        
        for (int i = 0 ; i < options.size(); i++) { // re- do option passing
            
            //            cout << options_prelim[i] << " ";
            
            evalOptions.push_back("x");
            evalOptions.push_back("10");
            
            // if -x or -p
            if (options[i].at(1) == 'x' || options[i].at(1) == 'p')
            {
                //                cout << "got here" << endl;
                // if -x
                if (options[i].at(1) == 'x') {
                    evalOptions.clear();
                    evalOptions.push_back("x");
                    evalOptions.push_back(options[i+1]); // within Evaluator, will need to cast these folds value into int
                    i++;
                }
                else if (options[i].at(1) == 'p') // if -p
                {
                    evalOptions.clear();
                    evalOptions.push_back("p");
                    evalOptions.push_back(options[i+1]); // within Evaluator, will need to cast these folds value into int
                    i++;
                }
                
            }
            else if (options[i].at(1) == 't')
            {
                trainFilename = options[i+1];
                i++;
            }
            else if (options[i].at(1) == 'T')
            {
                testFilename = options[i+1];
                testSet = true;
                i++;
            }
            else if (options[i].at(1) == 'k') // SET # random non-zero gain atttirubtes available to each split decision
            {
                //                cout << "HERE: l" << endl;
                RIOptions.push_back("k");
                RIOptions.push_back(options[i+1]);
                //                cout << options[i + 1] << " pushed back" << endl;
                i++;
            }
            else if (options[i].at(1) == 'b') // SET size random forest
            {
                //                cout << "HERE: i" << endl;
                RIOptions.push_back("b");
                RIOptions.push_back(options[i+1]);
                i++;
            }

            else
            {
                cout << "ERROR PARSING COMMAND LINE" << endl;
                return -1;
            }
            
            
        }

        DataSet train;
        DataSet test;
        
        train.load(trainFilename);
        
        ForestRI forRI(7, 3);
        
        forRI.setOptions(RIOptions);
        
        forRI.train(train);
        
        Performance perf;
        Evaluator eval;
        
        if (testSet) {
            test.load(testFilename);
            perf = eval.evaluate(forRI, train, test);
        }
        else
        {
            eval.setOptions(evalOptions);
            perf = eval.evaluate(forRI, train);
        }
        
        perf.print();
        
        
    } catch (string error) {
        cout << error;
    }
}