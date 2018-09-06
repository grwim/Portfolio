//
//  Performance.cpp
//  Project_4_MachineLearning
//
//  Created by Konrad Rauscher on 4/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Performance.hpp"

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

void Performance::setAccuracyList(vector<double> list)
{
    accuracies = list;
}

void Performance::print()
{
    
    if (accuracies.size() == 1) {
        cout << accuracies[0] << endl;
    }
    else
    {
        double sum;
        double mean;
        
        for (int i = 0; i < accuracies.size(); i++) {
            sum += accuracies[i];
        }
        mean = sum / accuracies.size();
        
        cout << mean << " +-" << stdDev << endl;
    }
    
}