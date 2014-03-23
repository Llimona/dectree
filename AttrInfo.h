#ifndef __Dectree__AttrInfo__
#define __Dectree__AttrInfo__

class AttrInfo {
    unordered_map<unsigned, unsigned> yValues;
    size_t recordCount;
public:
    AttrInfo();
    
    void addYValueID(unsigned yValueID);
    const double getEntropy() const;
    const size_t getRecordCount() const;
    const size_t getDistinctValues() const;
    const unsigned getMostCommonValueID() const;
};

#endif /* defined(__Dectree__AttrInfo__) */
