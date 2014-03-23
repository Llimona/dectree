//
//  DepAttrInfo.cpp
//  Dectree
//
//  Created by Eduard on 12/03/14.
//  Copyright (c) 2014 Eduard. All rights reserved.
//

#include "DepAttrInfo.h"

DepAttrInfo::DepAttrInfo() {
    values = unordered_map<unsigned, AttrInfo>();
    recordCount = 0;
}

void DepAttrInfo::addXYValues(unsigned xValID, unsigned yValID) {
    auto it = values.find(xValID);
    if (it != values.end()) {
        it->second.addYValueID(yValID);
    }
    else {
        AttrInfo yAttrInfo = AttrInfo();
        yAttrInfo.addYValueID(yValID);
        pair<unsigned, AttrInfo> element(xValID, yAttrInfo);
        values.insert(element);
    }
    ++recordCount;
}

const double DepAttrInfo::getDepEntropy() const {
    double result = 0.0;
    
    for (auto& x: values) {
        double entropy = x.second.getEntropy();
        double recordCount = x.second.getRecordCount();
        result += entropy * recordCount;
        //cout << x.first << " dep: " << entropy << " recordCount " << recordCount << endl;
    }
    
    result /= recordCount;
    
    //cout <<"depEntropy: " << result << " recordCount " << recordCount << endl;
    
    return result;
}

const size_t DepAttrInfo::getRecordCount() const {
    return recordCount;
}

const size_t DepAttrInfo::getDistinctValues() const {
    return values.size();
}

const size_t DepAttrInfo::getValueIDOccurrences
   (const unsigned valueID) const
{
    return values.find(valueID)->second.getRecordCount();
}