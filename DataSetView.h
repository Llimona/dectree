/**
 *
 *
 * Provides a partial view of a dataset maintaining lists of currently
 * visible records and attributes, providing operations to analize the
 * view's attributes. This object and its children shouldn't outlive the
 * DataSet object it's created from. */

#ifndef __Dectree__DataSetView__
#define __Dectree__DataSetView__

#include <iostream>

#include "DataSet.h"
#include "DepAttrInfo.h"


class DataSetView {
public:
    DataSetView(const DataSet &data, unsigned excludeAttrID);
    ~DataSetView();
    
    const bool checkAllRecordsEqual() const;
    const bool checkAttrIDValuesEqual(unsigned attributeID) const;
    const bool compareRecords(unsigned recordA, unsigned recordB) const;
    void subViews
       (unordered_map<unsigned, DataSetView*> &subViews,
        const vector<DepAttrInfo> &depAttrInfo,
        unsigned highestGainAttr) const;
    void findIndepAttrInfo
       (AttrInfo &indepAttrInfo,
        unsigned indepAttrID) const;
    void findDepAttrInfo
       (vector<DepAttrInfo> &depAttrInfo,
        unsigned indepAttrID) const;
    const double getAttributeGain
       (const DepAttrInfo &depAttrInfo,
        const AttrInfo &indepAttrInfo) const;
    const unsigned getHighestGainAttr
       (const vector<DepAttrInfo> &depAttrInfos,
        const AttrInfo &indepAttrInfo) const;
    
    const string& getAttributeName(unsigned attribute) const;
    const string& getValue(unsigned record, unsigned attribute) const;
    const string& getValueByID(unsigned valueID, unsigned attribute) const;
    const unsigned getAttributeID(unsigned attribute) const;
    const unsigned getValueIDbyAttrID(unsigned record, unsigned attributeID) const;
    
    const size_t nRecords() const;
    const size_t nAttributes() const;
private:
    class AttributeList {
        vector<unsigned> attributeIDs;
    public:
        // Initial construction, excluding the given element
        AttributeList(const DataSet &data,
                      const unsigned excludeAttrID);
        // List copy excluding the given element
        AttributeList(const AttributeList &oldAttrs,
                      const unsigned excludedAttr);

        const unsigned getAttributeID(unsigned attribute) const;
        const size_t size() const;
    };
    
    class RecordList {
        unsigned addedRecords;
        vector<unsigned> recordIDs;
    public:
        RecordList(size_t size);
        
        void addRecord(const unsigned recordID);
        const unsigned getRecordID(const unsigned record) const;
        const size_t size() const;
    };
    
    const DataSet *data;
    
    RecordList recordList;
    
    /* Stores the list of visible attributes for the view.
     
     Several DataSetView objects will point to a shared AttributeList
     object. To provide a simple and thread safe way to delete the
     AttributeList object once all the DataSetView instances pointing to
     it are deleted, the AttributeList is accessed through a smart
     pointer. */
    shared_ptr<AttributeList> attributeList;
    
    DataSetView(const DataSet *data,
                shared_ptr<AttributeList> &attrs,
                size_t nRecords);
    void addRecord(const unsigned recordID);
    
    const unsigned getValueID(unsigned record, unsigned attribute) const;
    
};

#endif /* defined(__Dectree__DataSetView__) */
