//
//  DecTree.h
//  Dectree
//
//  Created by Eduard on 12/03/14.
//  Copyright (c) 2014 Eduard. All rights reserved.
//

#ifndef __Dectree__DecTree__
#define __Dectree__DecTree__

#include <unordered_map>
#include <unordered_set>
#include <iostream>

#include "DataSet.h"
#include "AttrInfo.h"
#include "DepAttrInfo.h"
#include "DataSetView.h"
#include "WorkQueue.h"
#include "Thread.h"

class DecTree {
public:
    DecTree(const DataSet &data, unsigned outputAttrID);
    DecTree(const DataSet &data, unsigned outputAttrID, unsigned nThreads);
    void print(ostream &str, const DataSet &data) const;
    
private:
    unsigned outputAttrID;
    string targetAttrName;
    unsigned maxHeight;
    
    class TreeNode {
    protected:
        unsigned parentDecisionValID;
    public:
        void setParentDecisionValID(unsigned decisionValID);
        unsigned getParentDecisionValID();
        virtual unsigned getDecisionAttributeID();
        virtual unsigned getTargetDecisionValID();
        virtual TreeNode* getChild(unsigned index);
        virtual size_t nChildren();
    };
    
    class IntermediateTreeNode : public TreeNode {
        friend class TreeNode;
        
        unsigned decisionAttributeID;
        vector<TreeNode*> children;
    public:
        IntermediateTreeNode(unsigned decisionAttribute, size_t nChildren);
        
        unsigned getDecisionAttributeID();
        virtual TreeNode* getChild(unsigned index);
        void setChild(unsigned index, TreeNode* child);
        size_t nChildren();
    };
    
    class LeafTreeNode : public TreeNode {
        friend class TreeNode;
        
        unsigned targetDecisionValID;
    public:
        LeafTreeNode(unsigned decisionValID);
        
        unsigned getTargetDecisionValID();
    };
    
    TreeNode *root;
    
    void printBranch
       (ostream &str,
        TreeNode* node, vector<TreeNode*> &parentNodes,
        const DataSet &data,
        int depth) const;
    
    class WorkItem {
        const DataSetView* dsv;
        const unsigned splitValueID;
        IntermediateTreeNode *parent;
        const unsigned childIndex;
    public:
        WorkItem
           (const DataSetView* dsv,
            unsigned splitValueID,
            IntermediateTreeNode *parent,
            unsigned childIndex);
        ~WorkItem();
        
        const DataSetView* getDataSetView() const;
        const unsigned getSplitValueID() const;
        IntermediateTreeNode* getParent() const;
        const unsigned getChildIndex() const;
    };

    class BuilderThread : public Thread {
        unsigned threadNum;
        WorkQueue<WorkItem*> &workQueue;
        DecTree *tree;
        
        void buildTreeNode(WorkItem* work);
    public:
        BuilderThread(WorkQueue<WorkItem*> &workQueue, DecTree *tree);
        void run();
    };
    
};

#endif /* defined(__Dectree__DecTree__) */
