//
//  Performance.hpp
//  Project_4_MachineLearning
//
//  Created by Konrad Rauscher on 4/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#ifndef Performance_hpp
#define Performance_hpp

#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

class Performance
{
    
private:
    std::vector<double> accuracies; // list of k accuracies
    double stdDev;
    
public:
    Performance();
    
    void set_stdDev(double);
    void addAccuracy(double);
    void setAccuracyList(vector<double>);
    
    void print(); // prints average of k accuracies
    //
    
};

#endif /* Performance_hpp */
