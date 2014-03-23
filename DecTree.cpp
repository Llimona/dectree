//
//  DecTree.cpp
//  Dectree
//
//  Created by Eduard on 12/03/14.
//  Copyright (c) 2014 Eduard. All rights reserved.
//

#include "DecTree.h"

unsigned DecTree::TreeNode::getParentDecisionValID() {
    return parentDecisionValID;
}

void DecTree::TreeNode::setParentDecisionValID(unsigned decisionValID) {
    parentDecisionValID = decisionValID;
}

DecTree::IntermediateTreeNode::IntermediateTreeNode
   (unsigned decisionAttributeID, size_t nChildren) :
    decisionAttributeID(decisionAttributeID),
    children(vector<TreeNode*>(nChildren)) {}

void DecTree::IntermediateTreeNode::setChild(unsigned index, TreeNode* child) {
    children[index] = child;
}

DecTree::LeafTreeNode::LeafTreeNode(unsigned decisionValID) {
    targetDecisionValID = decisionValID;
}

unsigned DecTree::TreeNode::getDecisionAttributeID() {
    throw "Unapplicable operation.";
}

unsigned DecTree::IntermediateTreeNode::getDecisionAttributeID() {
    return decisionAttributeID;
}

DecTree::TreeNode* DecTree::TreeNode::getChild(unsigned index) {
    throw "Unapplicable operation.";
}

DecTree::TreeNode* DecTree::IntermediateTreeNode::getChild(unsigned index) {
    return children[index];
}

size_t DecTree::TreeNode::nChildren() {
    return 0;
}

size_t DecTree::IntermediateTreeNode::nChildren() {
    return children.size();
}

unsigned DecTree::TreeNode::getTargetDecisionValID() {
    throw "Unapplicable operation.";
}

unsigned DecTree::LeafTreeNode::getTargetDecisionValID() {
    return targetDecisionValID;
}

DecTree::DecTree(const DataSet &data, unsigned outputAttrID, unsigned nThreads) {
    this->outputAttrID = outputAttrID;
    DataSetView *dsv = new DataSetView(data, outputAttrID);
    maxHeight = (unsigned)data.nAttributes(); //maxheight
    
    vector<BuilderThread*> threads;
    threads = vector<BuilderThread*>(nThreads);
    WorkQueue<WorkItem*> workQueue;
    
    for (int i = 0; i < nThreads; ++i) {
        threads[i] = new BuilderThread(workQueue, this);
    }
    
    WorkItem* nextWork;
    nextWork = new WorkItem(dsv, 0, NULL, 0);
    workQueue.add(nextWork);

    for (int i = 0; i < nThreads; ++i) {
        threads[i]->start();
    }
    
    for (auto &thread : threads) {
        thread->join();
        delete(thread);
    }
}

DecTree::DecTree(const DataSet &data, unsigned outputAttrID) :
    DecTree(data, outputAttrID, 1) {}


void DecTree::BuilderThread::buildTreeNode(WorkItem* work) {
    TreeNode *node;
    
    const DataSetView* dsv = work->getDataSetView();
    const unsigned outputAttrID = tree->outputAttrID;

    if (dsv->checkAttrIDValuesEqual(outputAttrID)) {
        // All output values are equal: Predict its value
        node = new LeafTreeNode(dsv->getValueIDbyAttrID(0, outputAttrID));
    }
    else if (dsv->checkAllRecordsEqual()) {
        // All input values are equal: Predict majority output
        AttrInfo indepAttrInfo;
        dsv->findIndepAttrInfo(indepAttrInfo, outputAttrID);
        node = new LeafTreeNode(indepAttrInfo.getMostCommonValueID());
    }
    else {
        // Calculate attribute information from current records
        AttrInfo indepAttrInfo;
        dsv->findIndepAttrInfo(indepAttrInfo, outputAttrID);
        vector<DepAttrInfo> depAttrInfos;
        dsv->findDepAttrInfo(depAttrInfos, outputAttrID);
        
        // Use attribute information to find the highest gain attribute
        unsigned highestGainAttr;
        highestGainAttr = dsv->getHighestGainAttr(depAttrInfos, indepAttrInfo);

        // Create subviews removing the highest gain attribute
        unordered_map<unsigned, DataSetView*> subViews;
        dsv->subViews(subViews, depAttrInfos, highestGainAttr);

        // Recursively construct the children based on the partial record sets
        IntermediateTreeNode *iNode;
        iNode = new IntermediateTreeNode
           (dsv->getAttributeID(highestGainAttr),
            subViews.size());

        unsigned i = 0;
        for (auto& subView : subViews) {
            WorkItem* nextWork;
            nextWork = new WorkItem(subView.second, subView.first, iNode, i);
            workQueue.add(nextWork);
            ++i;
        }
        
        node = iNode;
    }
    
    IntermediateTreeNode *parent = work->getParent();
    if (parent != NULL) {
        parent->setChild(work->getChildIndex(), node);
        node->setParentDecisionValID(work->getSplitValueID());
    }
    else {
        tree->root = node;
    }
}

DecTree::BuilderThread::BuilderThread
   (WorkQueue<WorkItem*> &workQueue, DecTree *tree) :
    workQueue(workQueue), tree(tree) {}

void DecTree::BuilderThread::run() {
    WorkItem* workItem = workQueue.remove();
    while (workItem != NULL) {
        buildTreeNode(workItem);
        workQueue.closeItem();
        delete workItem;
        workItem = workQueue.remove();
    }
}

void DecTree::print(ostream &str, const DataSet &data) const {
    //cout << "print" << endl;
    vector<TreeNode*> parentNodes = vector<TreeNode*>(maxHeight+1);
    printBranch(str, root, parentNodes, data, 0);
}

void DecTree::printBranch
   (ostream &str,
    TreeNode* node,
    vector<TreeNode*> &branchNodes,
    const DataSet &data,
    int depth) const
{
    size_t nChildren = node->nChildren();
    branchNodes[depth] = node;
    if (nChildren == 0) {
        // Print branch
        for (int i = 0; i <= depth; ++i) {
            TreeNode *currentNode = branchNodes[i];
            size_t nChildren = currentNode->nChildren();
            if (i > 0) {
                unsigned parentDecisionAttrID = branchNodes[i-1]->getDecisionAttributeID();
                cout << data.getValueByID(currentNode->getParentDecisionValID(), parentDecisionAttrID);
            }
            if (nChildren > 0) {
                if (i > 0) cout << " & ";
                unsigned parentDecisionAttrID = currentNode->getDecisionAttributeID();
                cout << data.getAttributeName(parentDecisionAttrID);
                cout << " = ";
            }
            else {
                cout << " => " << data.getAttributeName(outputAttrID) << " = ";
                cout << data.getValueByID(currentNode->getTargetDecisionValID(), outputAttrID);
            }
        }
        cout << endl;
    }
    else {
        for (unsigned i = 0; i < nChildren; ++i) {
            TreeNode *child = node->getChild(i);
            printBranch(str, child, branchNodes, data, depth+1);
        }
    }
}

DecTree::WorkItem::WorkItem
   (const DataSetView* dsv,
    unsigned splitValueID,
    IntermediateTreeNode *parent,
    unsigned childIndex) :
    dsv(dsv),
    splitValueID(splitValueID),
    parent(parent),
    childIndex(childIndex) {}

DecTree::WorkItem::~WorkItem() {
    delete dsv;
}

const DataSetView* DecTree::WorkItem::getDataSetView() const {
    return dsv;
}

const unsigned DecTree::WorkItem::getSplitValueID() const {
    return splitValueID;
}

DecTree::IntermediateTreeNode* DecTree::WorkItem::getParent() const {
    return parent;
}

const unsigned DecTree::WorkItem::getChildIndex() const {
    return childIndex;
}
