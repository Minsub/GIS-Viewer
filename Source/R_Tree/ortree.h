//
//  Copyright (C) 2004-2007 by Autodesk, Inc.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

/// \ingroup Geometry_Module

//-----------------------------------------------------------------------------
//
// FILE: ortree.h.
//
// PURPOSE: Declaration of the OpsRTree class. The OpsRTree class implements
//          the basic functionality of a 3-4-5 memory resident R-Tree (insert-
//          ion, deletion, and searching). As such, additional functionality is
//          expected to be added via (private) inheritance. For example, the
//          OpsPolygonRTree derives from OpsRTree adding additional function-
//          ality required to support lasso pick operations.
//
//-----------------------------------------------------------------------------

#ifndef _ORTREE_H_
#define _ORTREE_H_

#include "oobject.h"
#include "oextent.h"
#include <float.h>
#include <assert.h>


class OpsRTree : public OpsObject {
public:
    /// constructor/destructor

   OpsRTree();
   virtual ~OpsRTree();

    /// methods to insert and delete items from the R-Tree

    void Insert(const void *item, const OpsFloatExtent *itemExtent);
    BOOL Delete(const void *item, const OpsFloatExtent *itemExtent);

    /// methods to initialize a search of the R-Tree, and return pointers
    /// to items that intersect the search area

    void BeginSearch(const OpsFloatExtent *searchExt);
    const void *GetNextItem();

    /// classes and method to override the standard search behaviour (the
    /// IntersectsSearchPredicate implements the standard search behaviour,
    /// returning all items that intersect a search extent - it is implicitly
    /// used by the BeginSearch(const OpsFloatExtent *))

    class SearchPredicate {
    public:
        virtual BOOL Predicate(const OpsFloatExtent &itemExtent) const = 0;
    };

    class IntersectsSearchExtent : public SearchPredicate {
    public:
        void Initialize(const OpsFloatExtent &searchExtent);
        virtual BOOL Predicate(const OpsFloatExtent &nodeExtent) const;
    private:
        OpsFloatExtent m_searchExtent;
    };

    void BeginSearch(const SearchPredicate *predicate);

    /// method to (re)initialize the R-Tree to an empty state

    void Initialize();

    /// method to get the extent of all items inserted into the R-Tree

    OpsFloatExtent *GetExtent(OpsFloatExtent *extent) const;

protected:
    /// methods to find the minimum/maximum of two floats.

    float Min(float x1, float x2) const;
    float Max(float x1, float x2) const;

#if !defined(TESTAPP_ACCESS)  // test app given protected access to members
private:
#endif
    /// declaration of nested classes used in the R-Tree implementation

    struct ItemContainer {
		const void *m_item;           // pointer to the item
        OpsFloatExtent m_itemExtent;  // extent of the item
    };

    struct RTreeNode {
        enum {MaxChildren = 5, MinChildren = 3};

        int m_nodeLevel;              // level of the node
        OpsFloatExtent m_nodeExtent;  // node extent
        void *m_child[MaxChildren];   // pointers to child nodes/items
        int m_subTree;                // used to save search state

        void Initialize(int level);
        int GetNChildren() const;
        RTreeNode *GetChildNode(int index) const;
        ItemContainer *GetChildItem(int index) const;
        const OpsFloatExtent *GetChildExtent(int index) const;
        BOOL IsNodeFull() const;
        BOOL IsaLeafNode() const;
        void AddChild(void *child);
        void RemoveChild(int index);
        void RecomputeExtent();
        int GetChildIndex(void *childPtr) const;
    };

    template <class ObjType > class Allocator {
    public:
        Allocator(int blockSize);
        ~Allocator();

        void Initialize();

        ObjType *Allocate();
        void Free(ObjType *obj);

    private:
        ObjType **m_blockPool;      // pointers to blocks of objects
        int m_blockSize;            // size of blocks to allocate
        int m_nBlocks;              // number of blocks
        int m_maxBlocks;            // current size of m_blockPool
        int m_nextObjIndex;         // index of next free object
        ObjType *m_headFreeList;    // pointer to head of free list
        enum {PoolBlockSize = 16};  // size to grow m_blockPool by

        void InitializeMembers(int blockSize);
        void UnInitialize();
        void GetNewBlock();
    };

    typedef Allocator<RTreeNode> NodeAllocator;
    typedef Allocator<ItemContainer> ContainerAllocator;

    class NodePtrStack {
    public:
        NodePtrStack();
        ~NodePtrStack();

        void Push(RTreeNode *nodePtr);
        void Pop();
        RTreeNode *Top() const;
        RTreeNode *GetNodePtrAt(int index) const;
        void Reset();
        BOOL IsEmpty() const;
        int GetSize() const;

    private:
        int m_topOfStack;                // index of top node pointer
        RTreeNode **m_nodePtrArray;      // array of node pointers
        enum {NodePtrArraySize = 64};    // size of node pointer array
    };

    /// the following are needed so that the nested classes can reference
    /// one another

    friend struct RTreeNode;
    friend class Allocator<ItemContainer>;
    friend class NodePtrStack;
    friend class SearchStateStack;
    friend class Allocator<RTreeNode>;

    /// class members

    int m_rTreeHeight;                          // current height of the R-Tree
    RTreeNode *m_rootNode;                      // pointer to the root node
    NodePtrStack m_nodePtrStack;                // stack of pointers to nodes
    NodeAllocator m_nodeAllocator;              // node allocator
    ContainerAllocator m_containerAllocator;    // container allocator
    RTreeNode **m_eliminatedNodes;              // used by deletion methods
    const SearchPredicate *m_predicate;         // current search predicate
    IntersectsSearchExtent m_standardPredicate; // standard search predicate

    /// implementation methods

    void Insert(void *item, const OpsFloatExtent *itemExtent, int level);
    RTreeNode *ChooseNode(const OpsFloatExtent *itemExtent, int level);
    double GetNonZeroArea(double width, double height) const;
    int ChooseBestSubTree(const RTreeNode *node,
        const OpsFloatExtent *itemExtent) const;
    RTreeNode *SplitNode(RTreeNode *fullNode, void *childItem,
        const OpsFloatExtent *childExtent);
    const int *ChooseBestPartition(const OpsFloatExtent * const inExtents[5],
        OpsFloatExtent *outExtent1, OpsFloatExtent *outExtent2);
    void AdjustRTree(RTreeNode *newNode);
    int FindLeafNode(const void *item, const OpsFloatExtent *itemExtent);
    void CondenseRTree(int itemIndex);

    /// private to prevent access

    OpsRTree(const OpsRTree &);
    OpsRTree &operator=(const OpsRTree &);
};

#endif
