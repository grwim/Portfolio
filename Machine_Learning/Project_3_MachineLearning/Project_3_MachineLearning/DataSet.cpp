//
//  DataSet.cpp
//  Project_3_MachineLearning
//
//  Created by Konrad Rauscher on 3/16/16.
//  Copyright © 2016 Konrad Rauscher. All rights reserved.
//

#include "DataSet.hpp"
#include <cstdio>
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
    
    const int MAX_CHARS_PER_LINE = 5000;
    const int MAX_TOKENS_PER_LINE = 100;
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
//                    cout << token[i] << " ";
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

vector<DataSet> DataSet::partition(int folds) const  
{
    vector<DataSet> partitions;
    srand ( time(NULL) );
    // using folds
    
    int partitionSize = (this->getExamples().size() / folds);
    
    
    for (int i = 0; i < folds; i++) {                       // create partitions
        
        DataSet * partition = new DataSet(this->getAttributes());
        while (partition->getExamples().size() < partitionSize) {
            
            int randIndex = rand() % this->getExamples().size();
            partition->add(this->getExamples()[randIndex]);
            
        }
        partitions.push_back(*partition);
        
    }
    
    return partitions;
}

vector<DataSet> DataSet::splitByProportion(double proportion) const // for Hold-out
{
    vector<DataSet> partitions;
    srand ( time(NULL) );
    
    DataSet trainSet(attributes);
    DataSet testSet(attributes);
    
    vector<int> indicesAlreadyAdded;
    
    // calculate training set size
    double trainSetSize = (examples.size() * proportion);
    
    for (int i = 0; i < trainSetSize; i++) {
        int randIndex = rand() % this->getExamples().size();
        
        bool indexIsNew = true;
        
        // check that rand Index isnt in indices already added
        for (int j = 0; j < indicesAlreadyAdded.size(); j++) {
            if (randIndex == indicesAlreadyAdded[j]) {
                indexIsNew = false;
            }
        }
        
        if (indexIsNew) { // if it is, add to train set
            trainSet.add(this->getExamples()[randIndex]);
            indicesAlreadyAdded.push_back(randIndex);
            
        }
        else // if it isnt,  dont add and decrement i (do over)
        {
            --i;
        }
    }
    
    int testSetSize = examples.size() - trainSetSize;
    for (int i = 0; i < testSetSize; i++) {
        int randIndex = rand() % this->getExamples().size();
        
        bool indexIsNew = true;
        
        // check that rand Index isnt in indices already added
        for (int j = 0; j < indicesAlreadyAdded.size(); j++) {
            if (randIndex == indicesAlreadyAdded[j]) {
                indexIsNew = false;
            }
        }
        
        if (indexIsNew) { // if it is, add to train set
            testSet.add(this->getExamples()[randIndex]);
            indicesAlreadyAdded.push_back(randIndex);
            
        }
        else // if it isnt,  dont add and decrement i (do over)
        {
            --i;
        }
    }
    
    partitions.push_back(trainSet);
    partitions.push_back(testSet);
    
    return partitions;
}


int DataSet::getBestSplitAtt() const // NEW for P3
{
    
    int bestSplitAtt = 0;
    double bestGain = 0.00;
    
    for (int i = 0; i < attributes.getSize() - 1; i++) { // - 1 so as to ignore class label 
        
        double currGain = this->gain(i);
        
        if (currGain > bestGain) {
            bestSplitAtt = i;
            bestGain = currGain;
        }
    }
    
    return bestSplitAtt;
}


vector<DataSet> DataSet::splitOnAtt(int attribute) const // NEW for P3
{
    vector<DataSet> splitSet;
    
    int attSize = dynamic_cast<NominalAttribute *>(this->attributes.get(attribute))->getSize();
    
    for (int i = 0; i < attSize; i++) {
        DataSet * emptyDS = new DataSet(this->getAttributes());
        splitSet.push_back(*emptyDS);
    }

    for (int i = 0; i < this->examples.size() ; i++) {
        
        for (int j = 0; j < attSize; j++) { // iterate through all values of the specified attribute
            
            if (j == this->examples[i][attribute]) {
                splitSet[j].add(this->examples[i]);
            }
        }
    }
    
    return splitSet;
}

double DataSet::entropy() const // NEW for P3
{
    int numClassVals = dynamic_cast<NominalAttribute *>(getAttributes().getClassAttribute())->getSize();
    
    vector<double> p(numClassVals);

    for (int i = 0; i < this->examples.size(); i++) {

        for (int j = 0; j < p.size(); j++) {
            if (j == this->examples[i][this->attributes.getClassIndex()]) {
                p[j] = (p[j]+1);
            }
        }
    }
      
    for (int i = 0; i < p.size(); i ++)
    {
        p[i] = (p[i] / this->examples.size());
    }
    
    double entropy = 0;
    for (int i = 0; i < p.size(); i++) {
        
        
        double singleEnt = 0;
        if (p[i] != 0) {
            singleEnt = ((p[i] * log2(p[i])));
        }
        
        entropy += singleEnt;
    }
    entropy = 0 - entropy;
    
    return entropy;
    
}

double DataSet::gain(int attribute) const  // NEW for P3
{
    int sizeAtt = dynamic_cast<NominalAttribute *>(attributes.get(attribute))->getSize();
    
//    calculate entropty of original data set
    double entropyOrig = this->entropy();
    
//    make vector of vectors with # class values for size of row vector and sizeAtt as size of vector thatcontains the row
    vector<vector<int> > table(sizeAtt);
    vector<int> row(dynamic_cast<NominalAttribute *>(attributes.getClassAttribute())->getSize());
    
    for (int i = 0; i < table.size(); i++) {
        table[i] = row;
    }
    
//    make seperate vector with the number of examples per attValue, size of sizeAtt
    vector<int> examplesPer_attValue(sizeAtt);
    
    for (int i = 0; i < examples.size(); i++) {
        
        for (int j = 0; j < examplesPer_attValue.size(); j++) {
            
            if (examples[i][attribute] == j)
            {
                examplesPer_attValue[j] = examplesPer_attValue[j] + 1;
            }
        }
    }
    
//    split data sets on attribute
    vector<DataSet> splits = this->splitOnAtt(attribute);
    
//     calculate entropy for for each split, multiply each of these entropies by proportion of the occurence ( # examples w/ knobby tires / # all examples), add them together to EntropySum
    
    double entropySum = 0;
    
    for (int i = 0; i < splits.size(); i++) {
        
        if (examplesPer_attValue[i] != 0) {
            entropySum += ((examplesPer_attValue[i] / double(examples.size())) * splits[i].entropy());
        }
    }
    
    return (entropyOrig - entropySum);
    
}











