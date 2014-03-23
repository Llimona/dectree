//
//  DepAttrInfo.h
//  Dectree
//
//  Created by Eduard on 12/03/14.
//  Copyright (c) 2014 Eduard. All rights reserved.
//

#ifndef __Dectree__DepAttrInfo__
#define __Dectree__DepAttrInfo__

#include <unordered_map>
using namespace std;

#include "AttrInfo.h"

class DepAttrInfo {
    // Keeps track of each value of X and the necessary information to calculate
    // both the entropy of the dependent records as well as their count
    unordered_map<unsigned, AttrInfo> values;
    size_t recordCount;
public:
    DepAttrInfo();
    
    void addXYValues(unsigned xValID, unsigned yValID);
    const double getDepEntropy() const;
    const size_t getRecordCount() const;
    const size_t getDistinctValues() const;
    const size_t getValueIDOccurrences(const unsigned valueID) const;
};

#endif /* defined(__Dectree__DepAttrInfo__) */
