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

#include "nb-main.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    string fileOpenError = "";
    deque<string> options_prelim;
    vector<string> options;
    
    
    try {
        
        for (int i = 1; i < argc; i++) {
            string s(argv[i]);
            options_prelim.push_back(s);
        }
        
        int folds = -1;
        string filename;
        
        if (options_prelim[0].at(1) == 'x' || options_prelim[0].at(1) == 'X') {
            folds = atoi(options_prelim[1].c_str());
            
            options_prelim.pop_front();     // remove '-x'
            options_prelim.pop_front(); // remove x value
            
        }
        
        for (int i = 0; i < options_prelim.size(); i++) {
            options.push_back(options_prelim[i]);
        }
        
        if (options[0].at(1) == 't') {
            filename = options[1];

        }
        else
        {
            cout << "ERROR: no training file specified " << endl;
            return -1;
        }
        
        
        DataSet dSet;
        
        dSet.load(filename);
        
        NaiveBayes nB;
        
        for (int i = 0; i < dSet.getAttributes().getSize(); i++) {
            cout << dynamic_cast<NominalAttribute *>(dSet.getAttributes().get(i))->getSize() << endl;
        }
        
        Evaluator eval(folds);
        
        Performance perf = eval.evaluate(nB, dSet);
        
        perf.print();
        
        //tts.print();
        
    } catch (string error) {
        cout << error;
    }
    
    cout << "Ending Main" << endl;
    return 0;
    
}