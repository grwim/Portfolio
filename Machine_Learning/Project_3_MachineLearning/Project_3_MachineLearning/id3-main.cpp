//
//  id3-main.cpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//


//Make sure your implementations of k-NN and naive Bayes from p2 work with the changes to Evaluator. They should just work. Include these implementations with your submission for this project. Yes. Your Makefile must build all three executables.

//Extend the Evaluator class from p2 to perform evaluation using the hold-out method. Use the -p switch to let users specify the proportion if the examples to use as training data.
//Extend the Evaluator class from p2 to perform evaluation on a set of testing examples that a user provides using the -T switch
//how to do this???

// need to make makefile

#include "id3-main.hpp"

int main(int argc, const char * argv[]) {

    
    string fileOpenError = "";
    vector<string> options;
    bool testSet = false;
    vector<string> evalOptions;
    
    string trainFilename;
    string testFilename;
    
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
            else
            {
                cout << "ERROR PARSING COMMAND LINE" << endl;
                return -1;
            }
            
            
        }
        
        
//        
//        if (options_prelim[0].at(1) == 'x' || options_prelim[0].at(1) == 'X') {
//            folds = atoi(options_prelim[1].c_str());
//            
//            options_prelim.pop_front();     // remove '-x'
//            options_prelim.pop_front(); // remove x value
//            
//        }
//        
//        for (int i = 0; i < options_prelim.size(); i++) {
//            options.push_back(options_prelim[i]);
//        }
//        
//        if (options[0].at(1) == 't') {
//            trainFilename = options[1];
//            
//        }
//        else
//        {
//            cout << "ERROR: no training file specified " << endl;
//            return -1;
//        }
        
        
        DataSet train;
        DataSet test;
        
        train.load(trainFilename);
        
        ID3 id3;
        
//        cout << train.getExamples().size() << endl;
//        
//        cout << train.entropy() << endl;
//        cout << train.getBestSplitAtt() << endl;
//    
        id3.train(train);
        
        // now to reconfigure Evaluator to work kwith passing options
        
        // only pass options if testSet = false;
    
        Performance perf;
        Evaluator eval;
        
        if (testSet) {
            test.load(testFilename);
            perf = eval.evaluate(id3, train, test);
        }
        else
        {
            eval.setOptions(evalOptions);
            perf = eval.evaluate(id3, train);
        }
        
        perf.print();
        
//
//        cout << "classifications: " << endl;
//        for (int i = 0; i < train.getExamples().size(); i++) {
//            
//            if (train.getExamples()[i][train.getAttributes().getClassIndex()] == id3.classify(train.getExamples()[i])) {
//                cout << "correct" << endl;
//            }
//            else
//            {
//                cout << "false" << endl;
//            }
//        }
    
    
//
//        cout << "finished" << endl;
        
//        Evaluator eval(evalOptions);
        
        
////        
//        Evaluator eval(folds); should change folds to evalOptions
////
//        Performance perf = eval.evaluate(id3, dSet);
        
//        Performance perf = eval.evaluate(id3, trainSet, testSet);
//
//        perf.print();
        
        //tts.print();
        
    } catch (string error) {
        cout << error;
    }
}