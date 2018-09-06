//
//  TrainTestSets.cpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "TrainTestSets.hpp"

TrainTestSets::TrainTestSets()
{
    
}

TrainTestSets::TrainTestSets( const vector<string> &options ) throw ( std::logic_error )
{
    setOptions(options);
}

DataSet TrainTestSets::getTrainingSet() const
{
    return train;
}

DataSet TrainTestSets::getTestingSet() const
{
    return test;
}

void TrainTestSets::setTrainingSet( const DataSet &myTrain )
{
    train = myTrain;
}

void TrainTestSets::setTestingSet( const DataSet &myTest )
{
    test = myTest;
}

void TrainTestSets::setOptions( const vector<string> &options ) throw ( logic_error )
{
    
    char c;
    //    cout << "options size is: " << options.size() << endl;
    for (unsigned int i = 0; i < options.size(); i++) {
        //        cout << "i is: " << i << " ";
        int a = 0;
        
        if (options[i].at(0) == '-')
        {
            
            c = options[i].at(++a); // take off '-'
            
            switch (c) {
                case 't':
                    //                    cout << options[i] << " caused: ";
                    //                    cout << "in train: " << options[1+i] << endl;
                    train.load(options[i+1]);
                    //                    cout << "Train set." << endl;
                    break;
                case 'T':
                    //                    cout << options[i] << " caused: ";
                    //                    cout << "in test: " << options[1+i] << endl;
                    test.load(options[i+1]);
                    //                    cout << "Test set." << endl;
                    
                    break;
                    
                default:
                    break;
            }
        }
    }
}

std::ostream &operator<<( std::ostream &out, const TrainTestSets &tts )
{
    //out << "DataSet train: " << endl;
    //out << tts.train;
    
    out << "PLEASE USE TrainTestSets::print() for TrainTestSets" << endl;
    
    //out << "DataSet test: " << endl;
    //out << tts.test;
    
    return out;
}

void TrainTestSets::print() const
{
    
    cout << "DataSet train: " << endl;
    train.print();
    
    cout << "DataSet test: " << endl;
    test.print();
    
}