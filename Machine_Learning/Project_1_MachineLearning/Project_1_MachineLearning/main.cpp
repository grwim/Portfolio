//
// Konrad Rauscher
// kr623@georgetown.edu
// Platform: OS X
// Language/Environment: c++
//
// In accordance with the class policies and Georgetown's Honor Code,
// I certify that, with the exceptions of the class resources and those
// items noted below, I have neither given nor received any assistance
// on this project.
//



#include "main.h"

using namespace std;

/*
 
 //load vector<string>options
 
 Classifier *classifier = new knn(options);
 
 Performance performance = Evaluator.evaluate(classifier, options);
 
 cout<<performance<<endl;
 
 */


// need to talk to Maloof about Mushrooms set apparently working

// Can only implement KNN and Naive Bayes for nominal values?? 

// possible issue of not being able to handle multiple command line arguments!



int main(int argc, const char * argv[]) {
    
    string fileOpenError = "";
    vector<string> options;
    
    try {
        
        for (int i = 1; i < argc; i++) {
            string s(argv[i]);
            options.push_back(s);
        }
        
        TrainTestSets tts;
        
        tts.setOptions(options);
        
        tts.print();
        
    } catch (string error) {
        cout << error;
    }
    
    cout << "Ending Main" << endl;
    return 0;
    
    
    // problem in between main and TrainTestSets.
    // need to upload on gergetown.cs server
    // or can just use DataSet:load directly 
    
}








