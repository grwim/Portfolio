//
//  AttributeFactory.cpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "AttributeFactory.hpp"

Attribute * AttributeFactory::make( const vector<string> &data ) throw ( logic_error )
{
    
    if (data[1] == "numeric") { // attribute is numeric
        
        
        Attribute * numAtt = new NumericAttribute(data[0]);
        
        //        cout <<  "New num is: ";
        //        dynamic_cast<NumericAttribute *>(numAtt)->print();
        //        cout << endl;
        return numAtt;
        
        
    }
    else{
        Attribute * nomAtt = new NominalAttribute(data[0]);
        
        //        cout << "added values: " << endl;
        for (unsigned int i = 1; i < data.size(); i++) {
            dynamic_cast<NominalAttribute *>(nomAtt)->addValue(data[i]);
            //cout << data[i] << " ";
        }
        
        return nomAtt;
        
    }
}