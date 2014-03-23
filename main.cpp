//
//  main.cpp
//  Dectree
//
//  Created by Eduard on 12/03/14.
//  Copyright (c) 2014 Eduard. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "DataSet.h"
#include "DataSetView.h"
#include "DecTree.h"

#define MIN_ARG_COUNT 3
#define MAX_ARG_COUNT 4
#define INPUT_ARG_INDEX 1
#define ATTR_ARG_INDEX 2
#define NTHREADS_ARG_INDEX 3

void printUsage() {
    cout << "Usage: dectree input_file output_attribute_index [n_threads]" << endl;
}

int main(int argc, const char *argv[]) {
    
    // Check arguments
    
    if (argc < MIN_ARG_COUNT) {
        cout << "Not enough arguments!" << endl;
        printUsage();
        return 0;
    }
    
    if (argc > MAX_ARG_COUNT) {
        cout << "Too many arguments!" << endl;
        printUsage();
        return 0;
    }
    
    ifstream inputFile (argv[INPUT_ARG_INDEX]);
    if (!inputFile.is_open()) {
        cout << "Invalid input file!" << argv[INPUT_ARG_INDEX] << endl;
        printUsage();
        return 0;
    }
    
    int attrIndex = -1;
    attrIndex = atoi(argv[ATTR_ARG_INDEX]);
    if (attrIndex == -1) {
        cout << "Invalid argument index!" << endl;
        printUsage();
        return 0;
    }
    
    int nThreads = -1;
    if (argc == MAX_ARG_COUNT) {
        nThreads = atoi(argv[NTHREADS_ARG_INDEX]);
        if (nThreads < 1) {
            cout << "Invalid number of threads, must provide a numerical value greater than 0." << endl;
            printUsage();
            return 0;
        }
    }
    else nThreads = 1;
    
    // Read Input File
    
    DataSet data = DataSet(inputFile);
    
    cout << "Attributes: " << data.nAttributes();
    cout << " Records: " << data.nRecords() << endl;
    
    if (data.nAttributes() < 2) {
        cout << "The value set requires at least two attributes!" << endl;
        return 0;
    }
    
    if (attrIndex < 0 || attrIndex >= data.nAttributes()) {
        cout << "The given attribute index is out of bounds!" << endl;
        printUsage();
        return 0;
    }
    
    if (data.nRecords() == 0) {
        cout << "The given dataset contains no records!" << endl;
        return 0;
    }
    
    // Treat Data
    
    DecTree tree = DecTree(data, attrIndex, nThreads);
    
    // Print Data
    
    tree.print(cout, data);

    return 0;
}

