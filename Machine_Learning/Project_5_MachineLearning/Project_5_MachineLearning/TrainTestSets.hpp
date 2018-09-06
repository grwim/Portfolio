//
//  TrainTestSets.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef TrainTestSets_hpp
#define TrainTestSets_hpp

#include <iostream>
#include "DataSet.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <stdexcept>


class TrainTestSets {
    
    friend std::ostream &operator<<( std::ostream &out, const TrainTestSets &tts );
    
protected:
    
    DataSet train;
    
    DataSet test;
    
public:
    
    TrainTestSets();
    
    TrainTestSets( const vector<string> &options ) throw ( std::logic_error );
    
    DataSet getTrainingSet() const;
    
    DataSet getTestingSet() const;
    
    void setTrainingSet( const DataSet &train );
    
    void setTestingSet( const DataSet &test );
    
    void setOptions( const vector<string> &options ) throw ( logic_error );
    
    void print() const;
    
}; // TrainTestSets class


#endif /* TrainTestSets_hpp */


