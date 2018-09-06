//
//  DataSet.cpp
//  Project_1_MachineLearning
//
//  Created by Konrad Rauscher on 1/31/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include <cstdio>
#include "DataSet.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <string.h>
#include <cstring>

using namespace std;


DataSet::DataSet()
{
    seed = 2026875034;
}

DataSet::DataSet( const Attributes &myAttributes) throw ( logic_error )
{
    attributes = myAttributes;
    seed = 2026875034;
}

void DataSet::add( const DataSet &dataset ) throw ( logic_error ) // adds examples of dataSet
{
    for (unsigned int i = 0; i < dataset.examples.size(); i++) {
        examples.push_back(dataset.examples[i]);
    }
    
    attributes = dataset.getAttributes();
    
}

void DataSet::add( const Example &example ) throw ( logic_error ) // adds example
{
    examples.push_back(example);
}

Attributes DataSet::getAttributes() const
{
    return attributes;
}

Examples DataSet::getExamples() const
{
    return examples;
}


long DataSet::getSeed() const
{
    return seed;
}


bool DataSet::getHasNominalAttributes() const
{
    return getAttributes().getHasNominalAttributes();
}

bool DataSet::getHasNumericAttributes() const
{
    return getAttributes().getHasNumericAttributes();
}

void DataSet::load( string filename ) throw ( logic_error )
{
    
    
    try {
        ifstream inFile(filename.c_str()); 
        string fileOpenError;
        //(filename.c_str());
        if (!inFile)
        {
//            inFile.close();
            fileOpenError = "Input file containing mine data: ";
            fileOpenError += filename;
            fileOpenError += " failed to open.";
            throw fileOpenError;
        }
        else
        {
            parse( inFile );
            inFile.close();
        }

    } catch (string sErr) {
        sErr = "Error in DataSet::load( string filename) ";
        throw sErr;
    }
}

void DataSet::setOptions( const vector<string> &options ) throw ( logic_error )
{
    // DID NOT IMPLEMENT - SEEMS TO BE FOR FUTURE PROJECTS
}


void DataSet::setSeed( long mySeed )
{
    seed = mySeed;
}


void DataSet::parse( ifstream &in ) throw ( logic_error )  // Parses a data set from the specified input stream, which consists of parsing the data set's header, attributes, and examples.
{
    
    const int MAX_CHARS_PER_LINE = 512;
    const int MAX_TOKENS_PER_LINE = 50;
    const char* const DELIMITER = " ";
    
    // read each line of the file
    while (!in.eof())
    {
        // read an entire line into memory
        char buf[MAX_CHARS_PER_LINE];
        in.getline(buf, MAX_CHARS_PER_LINE);
        
        // parse the line into blank-delimited tokens
        int n = 0; // a for-loop index
        
        // array to store memory addresses of the tokens in buf
        const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
        
        // parse the line
        token[0] = strtok(buf, DELIMITER); // first token
        
        if (token[0]) // zero if line is blank
        {
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(0, DELIMITER); // subsequent tokens
                if (!token[n]) break; // no more tokens
            }
            
            
        }
        
//        char keyDataSet[] = "@dataset\n"; // NEEDS TO BE NULL TERMINATED
//        
//        // process (print) the tokens
//        
//        for (int i = 0; i < n; i++) // n = #of tokens
//            cout << "Token[" << i << "] = " << token[i] << endl;
//        cout << endl;
//         cout << "HERE" << endl;

        const char *key_dataset = "@dataset";
        const char *key_attributes = "@attributes";
        const char *key_examples = "@examples";
        
        if (token[0]) // check that line is not empty
        {
            if (strncmp ( token[0], key_dataset,  8 ) == 0)
            {
                //cout << "got to dataset" << endl;
                name = token[1];
                //cout << "name is now: " << name << endl;
            }
            else if (strncmp ( token[0], key_attributes,  8 ) == 0)
            {
                //cout << "got to attributes!" << endl;
                // create a vector of strings, push each index of tokens on, pass vactor into attributes::parse() - need to change paramters of parse
                
                vector<string> attributeData;
                
                for (int i = 1; i < n; i++) {// n = #of tokens
                    attributeData.push_back(token[i]);
//                        cout << token[i] << " ";
                }
                
//                cout << endl;
                
                attributes.parse(attributeData);
    
            }
            else if (strncmp ( token[0], key_examples, 8 ) == 0)
            {

//                cout << "attributes are: ";
//                attributes.print();
                
                
                in.getline(buf, MAX_CHARS_PER_LINE);
                
                examples.setAttributes(attributes);
                
                while (!in.eof())
                {
                    // read an entire line into memory
                    //char buf[MAX_CHARS_PER_LINE];
                    in.getline(buf, MAX_CHARS_PER_LINE);
                    
                    // parse the line into blank-delimited tokens
                    int n = 0; // a for-loop index
                    
                    // array to store memory addresses of the tokens in buf
                    //const char* tokens[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
                    
                    // parse the line
                    token[0] = strtok(buf, DELIMITER); // first token
                    
                    if (token[0]) // zero if line is not blank
                    {
                        for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
                        {
                            token[n] = strtok(0, DELIMITER); // subsequent tokens
                            if (!token[n]) break; // no more tokens
                        }
                        
                        
                    }

                    vector<string> examplesData;
//                    cout << "example of size: " << n << endl;
                    for (int i = 0; i < n; i++) { // n = #of tokens       // Why is this not printing???
                        examplesData.push_back(token[i]);
//                        cout  << token[i] << " ";
                    }
//                    cout << endl;

                    if (n > 0) {
                    examples.parse(examplesData);
                    }

                }
 
            }
        }
    }
//    cout << "DataSet " << name << " loaded as ";
}

string DataSet::getName() const
{
    return name;
}

ostream & operator<<( ostream &out, const DataSet &ds )
{
    ds.print();
    
    return out;
}

void DataSet::clearExamples()
{
    examples.clear();
}

void DataSet::print() const
{
    cout << "@dataset " << getName() << endl << endl;
    
    if (getHasNominalAttributes() || getHasNumericAttributes()) {
        getAttributes().print();
        
        getExamples().print();
    }
    else {
        cout << "(empty)" << endl;
    }
    
    cout << endl;
}



