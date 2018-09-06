//
//  Performance.cpp
//  Project_2_MachineLearning
//
//  Created by Konrad Rauscher on 2/24/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Performance.h"

Performance::Performance()
{
    
}


void Performance::set_stdDev(double myDev)
{
    stdDev = myDev;
    
}


void Performance::addAccuracy(double acc)
{
    accuracies.push_back(acc);
}

void Performance::addAccuracyList(vector<double> list)
{
    accuracies = list;
}

void Performance::print()
{
    // calculates average, prints with stdDev 
    
    double sum;
    double mean;
    
    for (int i = 0; i < accuracies.size(); i++) {
        sum += accuracies[i];
    }
    mean = sum / accuracies.size();
    
    cout << mean << " +-" << stdDev << endl;
    
}