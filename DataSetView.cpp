#include "DataSetView.h"

DataSetView::DataSetView
   (const DataSet &data, const unsigned excludeAttrID) :
    data(&data),
    recordList(RecordList(data.nRecords())),
    attributeList(new AttributeList(data, excludeAttrID))
{
    for (unsigned recordID = 0; recordID < data.nRecords(); ++recordID) {
        recordList.addRecord(recordID);
    }
}

DataSetView::DataSetView
   (const DataSet *data,
    shared_ptr<AttributeList> &attrs,
    const size_t nRecords) :
    data(data),
    attributeList(attrs),
    recordList(RecordList(nRecords)) {}

DataSetView::~DataSetView() {
    attributeList.reset();
}

DataSetView::AttributeList::AttributeList
   (const AttributeList &oldAttrs,
    const unsigned excludedAttr) :
    attributeIDs(oldAttrs.size()-1)
{
    size_t newSize = oldAttrs.size()-1;
    for (unsigned i = 0; i < excludedAttr; ++i) {
        attributeIDs[i] = oldAttrs.getAttributeID(i);
    }
    for (unsigned i = excludedAttr; i < newSize; ++i) {
        attributeIDs[i] = oldAttrs.getAttributeID(i+1);
    }
}

DataSetView::AttributeList::AttributeList
   (const DataSet &data,
    const unsigned excludeAttrID) :
    attributeIDs(data.nAttributes()-1)
{
    size_t newSize = data.nAttributes()-1;
    for (unsigned i = 0; i < excludeAttrID; ++i) {
        attributeIDs[i] = i;
    }
    for (unsigned i = excludeAttrID; i < newSize; ++i) {
        attributeIDs[i] = i+1;
    }
}

void DataSetView::subViews(unordered_map<unsigned, DataSetView*> &subViews,
                           const vector<DepAttrInfo> &depAttrInfo,
                           const unsigned highestGainAttr) const
{
    unsigned highestGainAttrID = attributeList->getAttributeID(highestGainAttr);
    size_t attrDistinctValues = depAttrInfo[highestGainAttr].getDistinctValues();
    subViews = unordered_map<unsigned, DataSetView*>(attrDistinctValues);
    
    shared_ptr<AttributeList> subAttrList(
        new AttributeList(*attributeList, highestGainAttr));
    
    for (unsigned listIndex = 0; listIndex < recordList.size(); ++listIndex) {
        unsigned recordID = recordList.getRecordID(listIndex);
        unsigned valueID = data -> getValueID(recordID, highestGainAttrID);
        auto it = subViews.find(valueID);
        if (it != subViews.end()) {
            it->second->addRecord(recordID);
        }
        else {
            size_t nSubViewRecords;
            nSubViewRecords = depAttrInfo[highestGainAttr].getValueIDOccurrences(valueID);
            DataSetView *dsv = new DataSetView(data, subAttrList, nSubViewRecords);
            dsv->addRecord(recordID);
            subViews.insert(pair<unsigned, DataSetView*>(valueID, dsv));
        }
    }
}

void DataSetView::findDepAttrInfo
   (vector<DepAttrInfo> &depAttrInfo,
    unsigned indepAttrID) const
{
    size_t nAttributes = attributeList->size();
    size_t nRecords = recordList.size();
    
    depAttrInfo = vector<DepAttrInfo>(nAttributes);
    
    // For each record
    for (unsigned record = 0; record < nRecords; ++record) {
        // For each attribute
        for (unsigned attribute = 0; attribute < nAttributes; ++attribute) {
            unsigned xValueID = getValueID(record, attribute);
            unsigned yValueID = getValueIDbyAttrID(record, indepAttrID);
            depAttrInfo[attribute].addXYValues(xValueID, yValueID);
        }
    }
}

const bool DataSetView::compareRecords
   (unsigned recordA, unsigned recordB) const
{
    bool equal = true;
    unsigned attribute = 0;
    size_t nAttributes = this->nAttributes();

    while (attribute < nAttributes && equal) {
        equal = getValueID(recordA, attribute) == getValueID(recordB, attribute);
        ++attribute;
    }
    
    return equal;
}

const bool DataSetView::checkAllRecordsEqual() const {
    bool equal = true;
    unsigned record = 1;
    size_t nRecords = this->nRecords();
    
    while (record < nRecords && equal) {
        equal = compareRecords(0, record);
        ++record;
    }
    
    return equal;
}

const bool DataSetView::checkAttrIDValuesEqual(unsigned attributeID) const {
    unsigned record = 1;
    size_t nRecords = this->nRecords();
    bool equal = true;
    //if (attributeID >= nAttributes()) cout << "ERROR! attributeID: " << attributeID << " nAttributes: " << nAttributes() << endl;
    unsigned firstAttrValID = getValueIDbyAttrID(0, attributeID);
    
    while (record < nRecords && equal) {
        unsigned valID = getValueIDbyAttrID(record, attributeID);
        equal = firstAttrValID == valID;
        ++record;
    }
    
    return equal;
}

void DataSetView::findIndepAttrInfo
   (AttrInfo &indepAttrInfo,
    unsigned indepAttrID) const
{
    size_t nRecords = this->nRecords();
    
    for (unsigned record = 0; record < nRecords; ++record) {
        indepAttrInfo.addYValueID(getValueIDbyAttrID(record, indepAttrID));
    }
}

const double DataSetView::getAttributeGain
   (const DepAttrInfo &depAttrInfo,
    const AttrInfo &indepAttrInfo) const
{
    double independentEntropy = indepAttrInfo.getEntropy();
    double dependentEntropy = depAttrInfo.getDepEntropy();
    return independentEntropy - dependentEntropy;
}

const unsigned DataSetView::getHighestGainAttr
   (const vector<DepAttrInfo> &depAttrInfos,
    const AttrInfo &indepAttrInfo) const
{
    // Find attribute with highest information gain
    double maxGain = -1.0;
    int highestGainAttribute = -1;
    
    unsigned attribute = 0;
    size_t nAttributes = this->nAttributes();
    
    // For each attribute
    while (attribute < nAttributes) {
        double gain = getAttributeGain(depAttrInfos[attribute], indepAttrInfo);
        if (gain > maxGain) {
            highestGainAttribute = attribute;
            maxGain = gain;
        }
        ++attribute;
    }
    
    return highestGainAttribute;
}

DataSetView::RecordList::RecordList(size_t size)
: recordIDs(vector<unsigned>(size)), addedRecords(0) {}

void DataSetView::addRecord(const unsigned recordID) {
    recordList.addRecord(recordID);
}

void DataSetView::RecordList::addRecord(const unsigned recordID) {
    recordIDs[addedRecords] = recordID;
    ++addedRecords;
}

const unsigned DataSetView::RecordList::getRecordID
   (const unsigned listIndex) const
{
    return recordIDs[listIndex];
}

const size_t DataSetView::RecordList::size() const {
    return recordIDs.size();
}

const unsigned DataSetView::AttributeList::getAttributeID
   (unsigned listIndex) const
{
    return attributeIDs[listIndex];
}

const size_t DataSetView::AttributeList::size() const {
    return attributeIDs.size();
}

const string& DataSetView::getAttributeName
   (unsigned attribute) const
{
    unsigned attributeID = getAttributeID(attribute);
    return data->getAttributeName(attributeID);
}

const string& DataSetView::getValue
   (unsigned record, unsigned attribute) const
{
    unsigned recordID = recordList.getRecordID(record);
    unsigned attributeID = getAttributeID(attribute);
    return data->getValue(recordID, attributeID);
}

const string& DataSetView::getValueByID
   (unsigned valueID, unsigned attribute) const
{
    unsigned attributeID = getAttributeID(attribute);
    return data->getValueByID(valueID, attributeID);
}

const unsigned DataSetView::getValueID
   (unsigned record, unsigned attribute) const
{
    unsigned recordID = recordList.getRecordID(record);
    unsigned attributeID = getAttributeID(attribute);
    return data->getValueID(recordID, attributeID);
}

const unsigned DataSetView::getAttributeID(unsigned attribute) const {
    return attributeList->getAttributeID(attribute);
}

const unsigned DataSetView::getValueIDbyAttrID
   (unsigned record, unsigned attributeID) const
{
    unsigned recordID = recordList.getRecordID(record);
    return data->getValueID(recordID, attributeID);
}

const size_t DataSetView::nRecords() const {
    return recordList.size();
}

const size_t DataSetView::nAttributes() const {
    return attributeList->size();
}