//
//  Examples.cpp
//  Project_4_MachineLearning
//
//  Created by Konrad Rauscher on 4/3/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "Examples.hpp"

Examples::Examples()
{
    
}

Examples::Examples( const Attributes &myAttributes ) throw ( logic_error )
{
    attributes = myAttributes;
}

void Examples::parse( const vector<string> &data ) throw ( logic_error )
{
    Example * examplePtr = new Example(0);
    
    for (int i = 0; i < data.size(); i++) {
        
        if (typeid(*attributes.get(i)) == typeid(NominalAttribute))
        {
            
            //cout << "index of " << data[i] << " in attribute " << attributes.get(i)->getName() << " is: " << attributes.get(i)->getIndex(data[i]) << endl;
            
            // static cast to double, push onto Example
            examplePtr->push_back(static_cast<double>(attributes.get(i)->getIndex(data[i])));
            
            //            cout << i << "    " << data[i] << " is assigned an index of " << attributes.get(i)->getIndex(data[i]) << endl;
            
            
        }
        else if(typeid(*attributes.get(i)) == typeid(NumericAttribute))
        {
            
            //cout << "Numeric Attribute, so will push value of " << stod(data[i]) << " onto Example" << endl;
            
            // push onto Example
            //            double myDouble = stod(data[i]);
            
            examplePtr->push_back(stod(data[i]));
            
        }
        
    }
    
    //cout << "Size AFTER is: " << examplePtr->size();
    
    this->push_back(*examplePtr);
}

void Examples::setAttributes( const Attributes &myAttributes ) throw ( logic_error )
{
    attributes = myAttributes;
}

std::ostream &operator<<( std::ostream &out, const Examples &e )
{
    
    out << "@examples" << endl << endl;
    
    for (int i = 0; i < e.size(); i++) { // iterate through each Example
        
        //cout << "At Example: " << i << ", size: " << e.at(i).size() / 2 << endl;
        
        for (unsigned int j = 0; j < e.at(i).size(); j++) { // iterate through each double in Example i      ISSUE IS HERE: e.at(i).size()
            
            if (typeid(* e.attributes.get(j)) == typeid(NominalAttribute))   // ADD MORE INFORMATION TO TH ECOUT STATEMENTS TO MAKE THINGS CLEARER
            {
                out << dynamic_cast<NominalAttribute *>(e.attributes.get(j))->getValue(e.at(i).at(j)) << " ";
                
                //<< e.attributes.get(j)->getName() <<" "<<e.at(i).at(j) << " " <<
                
            }
            else if(typeid(* e.attributes.get(j)) == typeid(NumericAttribute))
            {
                out << e.at(i).at(j) << " ";
                
            }
        }
        out << endl;
    }
    return out;
}

void Examples::print() const
{
    cout << "@examples" << endl << endl;
    
    for (int i = 0; i < size(); i++) { // iterate through each Example
        
        //cout << "At Example: " << i << ", size: " << e.at(i).size() / 2 << endl;
        
        for (unsigned int j = 0; j < at(i).size(); j++) { // iterate through each double in Example i      ISSUE IS HERE: e.at(i).size()
            
            if (typeid(* attributes.get(j)) == typeid(NominalAttribute))   // ADD MORE INFORMATION TO TH ECOUT STATEMENTS TO MAKE THINGS CLEARER
            {
                cout << dynamic_cast<NominalAttribute *>(attributes.get(j))->getValue(at(i).at(j)) << " ";
                
                //<< e.attributes.get(j)->getName() <<" "<<e.at(i).at(j) << " " <<
                
            }
            else if(typeid(* attributes.get(j)) == typeid(NumericAttribute))
            {
                cout << at(i).at(j) << " ";
                
            }
        }
        cout << endl;
    }
}
