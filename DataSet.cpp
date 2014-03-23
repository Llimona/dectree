#define CSV_SEPARATOR ';'

#include <iostream>
#include <sstream>
using namespace std;

#include "DataSet.h"

DataSet::DataSet(istream &input) {
    records = vector<Record>();
    
    readHeaders(input);
    
    size_t nAttributes = attributeNames.size();
    valueNames = vector<vector<string>>(nAttributes);
    vector<unordered_map<string, unsigned>> valIDs;
    valIDs = vector<unordered_map<string, unsigned>>(nAttributes);
    vector<unsigned> nextIDs;
    nextIDs = vector<unsigned>(nAttributes, 0);
    
    while (readRecord(input, valIDs, nextIDs));
    
    for (size_t attributeID = 0; attributeID < nAttributes; ++attributeID) {
        size_t nValueNames = valIDs[attributeID].size();
        valueNames[attributeID] = vector<string>(nValueNames);
        for (auto& x: valIDs[attributeID]) {
            valueNames[attributeID][x.second] = x.first;
        }
    }
}

void DataSet::readHeaders(istream &input) {
    attributeNames = vector<string>();
    
    string line;
    getline(input, line);
    stringstream lineStream(line);
    string value;
    
    while (getline(lineStream, value, CSV_SEPARATOR)) {
        attributeNames.push_back(value);
    }
}

istream& DataSet::readRecord(istream &input,
                             vector<unordered_map<string, unsigned>> &valIDs,
                             vector<unsigned> &nextIDs)
{
    string line;
    getline(input, line);
    stringstream lineStream(line);
    string value;
    Record record(valIDs.size());
    
    unsigned attributeID = 0;
    while (getline(lineStream, value, CSV_SEPARATOR)) {
        unsigned valueID;
        auto it = valIDs[attributeID].find(value);
        if (it == valIDs[attributeID].end()) {
            valueID = nextIDs[attributeID];
            valIDs[attributeID][value] = valueID;
            nextIDs[attributeID] = valueID+1;
        }
        else {
            valueID = it->second;
        }
        record.setValueID(attributeID, valueID);
        ++attributeID;
    }
    if (attributeID == valIDs.size()) {
        // Insert record only if all of its attribute could be read
        records.push_back(record);
    }
    
    return input;
}

DataSet::Record::Record(size_t size) {
    valueIDs = vector<unsigned>(size);
}

void DataSet::Record::setValueID(unsigned attributeID, unsigned valueID) {
    valueIDs[attributeID] = valueID;
}

const unsigned DataSet::Record::getValueID(unsigned attributeID) const {
    return valueIDs[attributeID];
}

const string& DataSet::getAttributeName(unsigned attributeID) const {
    return attributeNames[attributeID];
}

const string& DataSet::getValue(unsigned recordID, unsigned attributeID) const {
    return valueNames[attributeID][getValueID(recordID, attributeID)];
}

const unsigned DataSet::getValueID(unsigned recordID, unsigned attributeID) const {
    return records[recordID].getValueID(attributeID);
}

const string& DataSet::getValueByID(unsigned valueID, unsigned attributeID) const {
    return valueNames[attributeID][valueID];
}

const size_t DataSet::nAttributes() const {
    return attributeNames.size();
}

const size_t DataSet::nRecords() const {
    return records.size();
}