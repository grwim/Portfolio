//
// Konrad Rauscher
// kr623@georgetown.edu
// Platform: MacOS
// Language/Environment: c++
//
// In accordance with the class policies and Georgetown's Honor Code,
// I certify that, with the exceptions of the class resources and those
// items noted below, I have neither given nor received any assistance
// on this project.
//

#include "bp-main.hpp"

int main(int argc, const char * argv[]) {

    
    string fileOpenError = "";
    vector<string> options;
    bool testSet = false;
    vector<string> evalOptions;
    
    string trainFilename;
    string testFilename;
    
    vector<string> BPOptions;
    
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
            else if (options[i].at(1) == 'l') // SET LEARNING RATE
            {
//                cout << "HERE: l" << endl;
                BPOptions.push_back("l");
                BPOptions.push_back(options[i+1]);
//                cout << options[i + 1] << " pushed back" << endl;
                i++;
            }
            else if (options[i].at(1) == 'i') // SET # INTERNAL NODES
            {
//                cout << "HERE: i" << endl;
                BPOptions.push_back("i");
                BPOptions.push_back(options[i+1]);
                i++;
            }
            else if (options[i].at(1) == 'm') // SET MIN ERROR RATE
            {
//                cout << "HERE: m" << endl;
                BPOptions.push_back("m");
                BPOptions.push_back(options[i+1]);
//                cout << options[i + 1] << " pushed back" << endl;
                i++;
            }
            else
            {
                cout << "ERROR PARSING COMMAND LINE" << endl;
                return -1;
            }
            
            
        }
        
//        cout << "BPOptions.size() is: " << BPOptions.size() << endl;
        
        DataSet trainData;
        DataSet testData;
 
        trainData.load(trainFilename);
        
//        for (int i = 0; i < trainData.getExamples().size(); i++) {
//            
//            for (int j = 0; j < trainData.getExamples()[i].size(); j++) {
//                cout << trainData.getExamples()[i][j] << " ";
//                
//            }
//            cout << endl;
//            
//        }
        
//        cout << "size examples: " << dataSet_adjusted.getExamples().size() << endl;
//        cout << "size example: " << dataSet_adjusted.getExamples()[0].size() << endl;
//        for (int i = 0; i < dataSet_adjusted.getExamples().size(); i++) {
//            
//            for (int j = 0; j < dataSet_adjusted.getExamples()[i].size(); j++) {
//                cout << dataSet_adjusted.getExamples()[i][j] << " ";
//                
//            }
//            cout << endl;
//            
//        }

        Performance perf;
        
        BP bp = * new BP();

        bp.setOptions(BPOptions);
//        bp.train(trainSet);
//        
//        perf = bp.classify(trainSet);
        
//        Evaluator eval;
//        eval.setOptions(evalOptions);
//        perf = eval.evaluate(bp, trainData);
        
//        perf = bp.classify(trainSet);
        
        Evaluator eval;
        
        if (testSet) {
            testData.load(testFilename);
            perf = eval.evaluate(bp, trainData, testData);
        }
        else
        {
            eval.setOptions(evalOptions);
            perf = eval.evaluate(bp, trainData);
        }
        
//        Performance perf =  bp.classify(train);
        
        perf.print();
        
        
        
        // NEED TO ALLOW FOR -T / TRAIN AND TEST SET UP .. look at P3
        
        
        
//        ID3 id3;
//        
//        //        cout << train.getExamples().size() << endl;
//        //
//        //        cout << train.entropy() << endl;
//        //        cout << train.getBestSplitAtt() << endl;
//        //
//        id3.train(train);
//        
//        // now to reconfigure Evaluator to work kwith passing options
//        
//        // only pass options if testSet = false;
//        

//        
//        //
//        //        cout << "classifications: " << endl;
//        //        for (int i = 0; i < train.getExamples().size(); i++) {
//        //
//        //            if (train.getExamples()[i][train.getAttributes().getClassIndex()] == id3.classify(train.getExamples()[i])) {
//        //                cout << "correct" << endl;
//        //            }
//        //            else
//        //            {
//        //                cout << "false" << endl;
//        //            }
//        //        }
//        
//        
//        //
//        //        cout << "finished" << endl;
//        
//        //        Evaluator eval(evalOptions);
//        
//        
//        ////        
//        //        Evaluator eval(folds); should change folds to evalOptions
//        ////
//        //        Performance perf = eval.evaluate(id3, dSet);
//        
//        //        Performance perf = eval.evaluate(id3, trainSet, testSet);
//        //
//        //        perf.print();
//        
//        //tts.print();
        
    } catch (string error) {
        cout << error << endl;
    }
}