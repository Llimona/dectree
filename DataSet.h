#ifndef __Dectree__DataSet__
#define __Dectree__DataSet__

#include <vector>
#include <unordered_map>
using namespace std;

class DataSet {
public:
    DataSet(istream& input);
    
    const string& getAttributeName(unsigned attributeID) const;
    const string& getValue(unsigned recordID, unsigned attributeID) const;
    const string& getValueByID(unsigned valueID, unsigned attributeID) const;
    const unsigned getValueID(unsigned recordID, unsigned attributeID) const;
    
    const size_t nAttributes() const;
    const size_t nRecords() const;
private:
    class Record {  
    public:
        Record(size_t size);
        
        void setValueID(unsigned attributeID, unsigned valueID);
        const unsigned getValueID(unsigned attributeID) const;
    private:
        vector<unsigned> valueIDs;
    };

    vector<string> attributeNames;
    vector<vector<string>> valueNames;
    vector<Record> records;

    istream& readRecord(istream &input,
                        vector<unordered_map<string, unsigned>> &valIDs,
                        vector<unsigned> &lastIDs);
    
    void readHeaders(istream &input);
};

#endif /* defined(__Dectree__DataSet__) */
