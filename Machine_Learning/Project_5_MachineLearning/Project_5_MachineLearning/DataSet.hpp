//
//  DataSet.hpp
//  Project_5_MachineLearning
//
//  Created by Konrad Rauscher on 4/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef DataSet_hpp
#define DataSet_hpp

#include <cstdio>
#include <iostream>
#include "Attributes.hpp"
#include "Examples.hpp"
#include <cmath>
#include <stdexcept>

class DataSet {
    
    friend ostream &operator<<( iostream &out, const DataSet &ds );
    
protected:
    
    string name;
    
    Attributes attributes;
    
    Examples examples;
    
    long seed;
    
public:
    
    DataSet();
    
    DataSet( const Attributes &attributes ) throw ( logic_error );
    
    void add( const DataSet &dataset ) throw ( logic_error );
    
    void add( const Example &example ) throw ( logic_error );
    
    Attributes getAttributes() const;
    
    Examples getExamples() const;
    
    long getSeed() const;
    
    bool getHasNominalAttributes() const;
    
    bool getHasNumericAttributes() const;
    
    void load( string filename ) throw ( logic_error );      // public side of parse
    
    void setOptions( const vector<string> &options ) throw ( logic_error );
    
    void setSeed( long seed );
    
    string getName() const;
    
    void clearExamples();
    
    void print() const;
    
    vector<DataSet> partition(int folds) const;
    
    int getBestSplitAtt() const; // NEW for P3
    
    vector<DataSet> splitOnAtt(int attribute) const; // NEW for P3
    
    double entropy() const;  // NEW for P3
    
    double gain(int attribute) const;  // NEW for P3
    
    vector<DataSet> splitByProportion(double proportion) const; // NEW for P3
    
    DataSet nominalToBinary() const;
    
    vector<DataSet> bootstrap(int k) const; // NEW for P5
    
    int getBestSplitAtt(vector<int> ) const; // NEW for P5
    
    
private:
    
    void parse( ifstream &in ) throw ( logic_error );  // Parses a data set from the specified input stream, which consists of parsing the data set's header, attributes, and examples.
    
}; // DataSet class


#endif /* DataSet_hpp */


