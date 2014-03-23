#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
using namespace std;

#include "AttrInfo.h"

AttrInfo::AttrInfo() {
    yValues = unordered_map<unsigned, unsigned>();
    recordCount = 0;
}

void AttrInfo::addYValueID(unsigned yValueID) {
    auto it = yValues.find(yValueID);
    if (it != yValues.end()) {
        it->second += 1;
    }
    else {
        pair<unsigned, unsigned> element(yValueID, 1);
        yValues.insert(element);
    }
    ++recordCount;
}

const double AttrInfo::getEntropy() const {
    double total = (double) recordCount;
    double entropy = 0.0;

    for (auto& x: yValues) {
        double prob = (double)x.second/total;
        entropy -= prob * log(prob)/log(2);
    }
    
    return entropy;
}

const size_t AttrInfo::getRecordCount() const {
    return recordCount;
}

const size_t AttrInfo::getDistinctValues() const {
    return yValues.size();
}

const unsigned AttrInfo::getMostCommonValueID() const {
    unsigned result = 0;
    unsigned maxCount = 0;
    for (auto& x: yValues) {
        if (x.second > maxCount) {
            maxCount = x.second;
            result = x.first;
        }
    }
    return result;
}