#ifndef DATANODE_HPP
#define DATANODE_HPP

#include<vector>

template<class T>
class DataNode {

    private:

    DataNode();
    DataNode(T const&);
    DataNode(DataNode<T> *,int);

    ~DataNode();

    /*
    counters of the node
    */
    int _numElems;
    int _numChild;
    int _numDeepth;

    bool __ISLEAF__;
    bool __ISROOT__;
    /*
    data vector
    */
    std::vector<T> _elems;

    /*
    children's pointers vector
    */
    std::vector<DataNode<T>*> _child;

    /*
    head node pointer of the tree
    */
    DataNode<T> * _treeHead;

    /*
    pointer of the parent
    */
    DataNode<T> * _parent;

    /*
    left and right brothers's pointer
    */
    DataNode<T> * _leftNode;
    DataNode<T> * _rightNode;

    /*
    friend class for operate on this data class
    */
    template<class E> friend class BTreeImpl;

    template<class V> friend class DataNode;


    /*
    end of the class DataNode
    */
};



template<class T>
DataNode<T>::DataNode()
:_numElems(0),_numChild(0),_numDeepth(1),
_parent(NULL),_leftNode(NULL),_rightNode(NULL),
_treeHead(this),__ISLEAF__(true),__ISROOT__(true)
{
    /*
    create a node without message,usually will
    be called to create a head node,(when the
    head node need to devide)
    ------------------------------------------
    Feature of a new node:
    _numElems - 0;
    _numChild - 0;
    _leftNode - NULL;
    _rightNode - NULL;
    Feature of the head:
    deepth - 1;
    parent - NULL;
    treeHead - this;
    */
}

template<class T>
DataNode<T>::DataNode(T const& t)
:_numElems(1),_numChild(0),_numDeepth(1),
_parent(NULL),_leftNode(NULL),_rightNode(NULL),
_treeHead(this),__ISLEAF__(true),__ISROOT__(true)
{
   _elems.push_back(t);
}

template<class T>
DataNode<T>::DataNode(DataNode<T> * d,int where)
:_treeHead(d->_treeHead),_parent(d->_parent),
__ISLEAF__(d->__ISLEAF__),__ISROOT__(d->__ISROOT__),
_numDeepth(d->_numDeepth)
{
    /*
    create a new node from an old,usually will
    be called to copy a part from the old(num 
    of the elems over the limit,the old tree 
    need to devide into two part)
    -------------------------------------------
    Feature of the new part:
    _treeHead,_parent,_numDeepth - same as old;
    elems,child changed,especially the child's 
    _parent is changed.
    */

    typename std::vector<T>::iterator \
    elem_beg,elem_end;
    typename std::vector<DataNode<T>*>::iterator \
    chld_beg,chld_end;

    /*
    copy element from index 'where' to end
    */
    _numElems = 0;
    elem_beg = d->_elems.begin() + where;
    elem_end = d->_elems.end();
    while(elem_beg != elem_end) {
        _elems.push_back(*(elem_beg++));
        ++_numElems;
    }

    /*
    set left and right brother pointer
    */
    _leftNode = d;
    _rightNode = d->_rightNode;

    /*
    devide the children and update their _parent
    */
    _numChild = 0;
    if(__ISLEAF__) return;

    chld_beg = d->_child.begin() + where;
    chld_end = d->_child.end();
    while(chld_beg != chld_end) {
        (*chld_beg)->_parent = this;
        _child.push_back(*(chld_beg++));
        ++_numChild;
    }

    /*
    end of the DataNode(DataNode<T> *,int);
    */
}

template<class T>
DataNode<T>::~DataNode()
{
    /*
    Althrough there are many pointers value in
    this class,but there come from outside,the
    class DataNode itslef doesn't create any 
    object by operator 'new',so this is nothing
    to clear. 
    */
}

#endif