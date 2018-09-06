//
//  BP.cpp
//  Project_4_MachineLearning
//
//  Created by Konrad Rauscher on 4/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "BP.hpp"
using namespace std;

double e = 2.71828182845904523536;

int BP::classify(const Example & example)
{

    // grab example + propage it through the network: fowad pass
    // get O
    
    // return the index of the componenet of 0 that is maximum
    vector<double> y;
    vector<double> o;
    vector<double> z;
//    d.at(static_cast<int>(ds.getExamples()[i][ds.getAttributes().getClassIndex()])) = 1.0; // initialize d
    //            cout << "example " << i << ": ";
    for (int i = 0 ; i < example.size() - 1; i++) { // z       // MISSING VALUES HERE: -1, and index 0
        z.push_back(example[i]);
        
        //                cout << ds.getExamples()[i][j] << " ";
    }
    //            cout << endl;
    
    double sum = 0;
    y.clear();
    for (int j = 0; j < v.size(); j++) {
        
        //            cout << "here " << endl;
        for (int x = 0; x < z.size(); x++) {
            
            //                cout << z->at(x) << " ";
            sum += v.at(j).at(x) * z.at(x);
            
        }
        //            cout << endl;
        y.push_back(1 / (1 + pow(e, -1 * sum)));
        
    }
    y[y.size()-1] = -1;
    
//    cout << "y: ";
//                for (int f = 0; f < y.size(); f++) {
//                    cout << y.at(f) << " ";
//                }
//                cout << endl;
    
    // now for next layer
    sum = 0;
    o.clear();
//                cout << w.size() << endl;
    for (int k = 0; k < w.size(); k++) {
        
        for (int x = 0; x < y.size(); x++) {
            
            sum += w.at(k).at(x) * y.at(x);
            
        }
        o.push_back(1 / (1 + pow(e, -1 * sum)));
    }
    
//    cout << "o: ";
//    for (int i = 0; i < o.size(); i++) {
//        cout << o[i] << " ";
//    }
//    cout << endl;

    int indexMax = 0;
    double max = 0;
    for (int i = 0; i < o.size(); i++) {
        if (o[i] > max) {
            max = o[i];
            indexMax = i;
        }
    }

    return indexMax;
}

Performance BP::classify( const DataSet & ds)
{
//    cout << "got here " << endl;
    DataSet adjusted = ds.nominalToBinary();
    
    Performance performance;
    
    vector<int> predictions;
    
    vector<double> accuracies;
    
    // predict on test
    for (int i = 0; i < adjusted.getExamples().size(); i++) {
        predictions.push_back(this->classify(adjusted.getExamples()[i]));
//                    cout << "prediction: " << this->classify(adjusted.getExamples()[i]) << endl;
    }
    
    int countCorrect = 0;
    
    //                cout << "size of test is " << test.getExamples().size() << endl;
//    cout << "size of ds is " << ds.getExamples().size() << endl;
    
    for (int i = 0; i < adjusted.getExamples().size(); i++) { // get count of # correct
        
        //                cout << "prediction is " << predictions[i] << " and actual is " << test.getExamples()[i][test.getAttributes().getClassIndex()] << endl;
        
        if (predictions[i] == adjusted.getExamples()[i][adjusted.getAttributes().getClassIndex()]) {
            countCorrect++;
            //            cout << " correct " << endl;
        }
    }
//    cout << "countCorrect: " << countCorrect << endl;
//    cout << "Accuracy: " << countCorrect / static_cast<double>(adjusted.getExamples().size()) << endl;
    accuracies.push_back(countCorrect / static_cast<double>(adjusted.getExamples().size()));   // push back accuracy
    
    performance.setAccuracyList(accuracies);
    
    //            cout << "Evaluation  " << i << " with size " << train.getExamples().size() << " " << test.getExamples().size() << endl;
    
    return performance;
}

void BP::train(const DataSet & dsX)
{
//    double Error;
    
    DataSet ds = dsX.nominalToBinary();
    
    srand(time(NULL));
    
    // initialize v and w to small, random values
    
    int J = numInternNodes; // # internal nodes in hidden layer
    int I = (ds.getAttributes().getSize() - 1); // size of each example - 1 (exclude class attribute)
    int K = dynamic_cast<NominalAttribute *>(ds.getAttributes().getClassAttribute())->getSize(); // # of class values ??
    
//    cout << "J: " << J << endl;
    // V is (J x I),
    // W is (K X J)
    
    // initialize v to small random values
    v.clear();
    for (int i = 0; i < J; i++) {
        vector<double> doubleVect;
        
        for (int j = 0; j < I; j++) {
            
            double randVall = (rand() % 24 + 1) / 100.00;
            //            cout << randVall << " ";
            doubleVect.push_back(randVall);
        }
        v.push_back(doubleVect);
        //        cout << endl;
    }
    
    //    cout << v.size() << endl;
    //    for (int i = 0; i < v.size(); i++) {
    //
    //        for (int j = 0; j < I; j++) {
    //            cout << v[i][j] << " ";
    //        }
    //
    //    }
    //    cout << endl;
    // initialize w to small random values
    
    w.clear();
    for (int i = 0; i < K; i++) {
        vector<double> doubleVect;
        
        for (int j = 0; j < J; j++) {
            double randVall = (rand() % 24 + 1) / 100.00;
            //            cout << randVall << " ";
            doubleVect.push_back(randVall);
        }
        //        cout << endl;
        w.push_back(doubleVect);
    }
    

    // load z with all values except the class value
    
    // initialize weights to small random values
    
    
    //...
    
    // initilize the size of d from get-go
    // find max value for class value, then create d of size of this max value
    
    int maxVal = 0;
    
    for (int i = 0; i < dsX.getExamples().size(); i++) {
        
        if (dsX.getExamples()[i][dsX.getAttributes().getClassIndex()] > maxVal) {
//            cout << dsX.getExamples()[i][dsX.getAttributes().getClassIndex()] << " ";
            maxVal = dsX.getExamples()[i][dsX.getAttributes().getClassIndex()];
//            cout << endl;
            
        }
        
    }
    
//    cout << "maxval: " << maxVal << endl;
    
    vector<double> y;
    vector<double> o;
    
    double Error;

//    for (int i = 0; i < d->size(); i++) {
//        cout << d->at(i) << " ";
//    }
    int Epoch = 0;
    
    do {
        Epoch++;
        Error = 0;
        for (int i = 0; i < ds.getExamples().size(); i++) {
            
            vector<double> z; // initilize z
//            cout << "index of class value " << ds.getExamples()[i][ds.getAttributes().getClassIndex()] << endl;
            
            vector<double> d(maxVal + 1);
            
            d.at(ds.getExamples()[i][ds.getAttributes().getClassIndex()]) = 1.0; // initialize d
            
//            cout << "example " << i << ": ";
            for (int j = 0 ; j < ds.getExamples()[i].size() - 1; j++) { // z
                z.push_back(ds.getExamples()[i][j]);
                
//                cout << ds.getExamples()[i][j] << " ";
            }
//            cout << endl;
            
            double sum = 0;
            y.clear();
            for (int j = 0; j < v.size(); j++) {
                
                //            cout << "here " << endl;
                for (int x = 0; x < z.size(); x++) {
                    
//                                    cout << z.at(x) << " ";
                    sum += v[j][x] * z.at(x);
                    
                }
                //            cout << endl;
                y.push_back(1 / (1 + pow(e, -1 * sum)));
                
            }
            y[y.size()-1] = -1;
            
            
//            for (int f = 0; f < y.size(); f++) {
//                cout << y.at(f) << " ";
//            }
//            cout << endl;
            
            // now for next layer
            sum = 0;
            o.clear();
            
            for (int k = 0; k < K; k++) {
                
                for (int x = 0; x < J; x++) {
                    
                    sum += w.at(k).at(x) * y.at(x);
                    
                }
                o.push_back(1 / (1 + pow(e, -1 * sum)));
            }
            
//            cout << "o: ";
//            for (int f = 0; f < o.size(); f++) {
//                cout << o.at(f) << " ";
//            }
//            cout << endl;
//            
//            cout << "d: ";
//            for (int f = 0; f < d.size(); f++) {
//                cout << d[f] << " ";
//            }
//            cout << endl;
            
            // compute error value
            //    E=1(dk−ok)2+E, for k=1,2,...,K
            for (int q = 0; q < K; q++) {
//                            cout << d.at(w) << " " << o.at(w) << endl;
                Error += ((.5) * pow((d.at(q) - o.at(q)), 2));
                
            }
            
            // error signal vectors
            
            vector<double>errSigVect_o = vector<double>();
            vector<double>errSigVect_y = vector<double>();
            
            // output layer -- > δok = (dk −ok)(1−ok)ok, for k = 1,2,...,K.
            
            for (int k = 0; k < w.size(); k++) {
                
                //            cout << "errSigVect_o.size() is " << errSigVect_o.size() << ", " << "d[k] is " << d[k] << ", " << "d.size() is " << d.size() << ", " << "o[k] is " << o[k] << ", " << "o.size() is " << o.size() << endl;
                
                errSigVect_o.push_back((d[k] - o[k])*(1 - o[k]) * o[k]);
                
            }
            
//            cout << "errSigVect_o" << endl;
//            for (int k = 0; k < errSigVect_o.size(); k++) {
//                cout << errSigVect_o[k] << " ";
//            }
//            cout << endl;
            
            // hidden layer -- > δyj =yj(1−yj) K SIGMA k=1 δokwkj, forj=1,2,...,J.
            
            for (int j = 0; j < v.size(); j++) {
                
                double summation = 0;
                for (int k = 0; k < errSigVect_o.size(); k++) {
                    
                    summation += errSigVect_o[k] * w[k][j]; // is this right??
                    
                }
                
                errSigVect_y.push_back( (y[j]*(1-y[j])* summation));
                
            }
            
////            cout << "errSigVect_y" << endl;
//            for (int k = 0; k < errSigVect_y.size(); k++) {
////                cout << errSigVect_y[k] << " ";
//            }
////            cout << endl;
//            
            
            for (int k = 0; k < w.size(); k++) {
                
                for (int j = 0; j < v.size(); j++) {
                    
                    w[k][j] = (w[k][j] + learnRate * errSigVect_o[k]*y[j]);
                    
                }
            }
            
            for (int j = 0; j < v.size(); j++) {
                for (int i = 0; i < z.size(); i++) {
                    v[j][i] = v[j][i] + learnRate * errSigVect_y[j] * z[i];
                }
            }
        }
//        cout << "Epcoch " << Epoch << ", Error: " << Error << endl;
    } while (Error > minError);
    

//    cout << "v: ";
//    for (int i = 0; i < v.size(); i++) {
//        for (int j = 0; j < v[i].size(); j++) {
//            cout << v[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;
//    
//    cout << "w: ";
//    for (int i = 0; i < w.size(); i++) {
//        for (int j = 0; j < w[i].size(); j++) {
//            cout << w[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;
}

void BP::setOptions(vector<string> & options )
{
    
    // DEFAULT VALUES
    learnRate = .10; numInternNodes =11; minError = .1;
    
//    cout << "options.size() is " << options.size() << endl;
    
//    for (int i = 0; i < options.size(); i++) {
//        cout << options[i] << " ";
//    }
//    cout << endl;
    
    
    
    for (int i = 0; i < options.size(); i++) {
        if (options[i][0] == 'l') {
            i++;
            learnRate = stof(options[i]);
        }
        else if (options[i][0] == 'i')
        {
            i++;
            numInternNodes = stof(options[i]);
        }
        else if (options[i][0] == 'm')
        {
            i++;
            minError = stof(options[i]);
        }
    }
}



BP::BP()
{


}

