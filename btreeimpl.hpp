#ifndef BTREEIMPL_HPP
#define DTREEIMPL_HPP

#include"datanode.hpp"

template<class T>
class BTreeImpl {

    private:

    BTreeImpl();
    BTreeImpl(T const&);
    BTreeImpl(BTreeImpl<T> const&);
    ~BTreeImpl();

    /*
    outer operate implements
    ==================================================
    */
    int getHeight() const;

    DataNode<T> * getRoot() const;

    void insertElem(T const&);

    void deleteElem(T const&);

    void printTree() const;

    int countElems() const;

    void clearTree();

    T const * searchElem(T const&) const;

    /*
    inner auxiliary functions.
    ==================================================
    */
    
    int _updateHeight(int);

    DataNode<T> * _updateTHead();

    int _updateHeight(int &,DataNode<T> *);

    void _printBTree(DataNode<T> *) const;

    void _incHeight(DataNode<T> *,int);

    void _setRoot(DataNode<T> *,DataNode<T> *);

    bool _insertElem(T const&,DataNode<T> *);

    bool _deleteElem(T const&,DataNode<T> *);

    bool _devideBack(DataNode<T> *);

    bool _moveToBros(DataNode<T> *);

    void _release(DataNode<T> *&);

    bool _combine(DataNode<T> *);

    int __count__(DataNode<T> *) const;

    T const * _search(T const&,DataNode<T>*) const;


    /*
    inner data structre
    ==================================================
    */

    /*
    head pointer of the b tree
    */
    DataNode<T> * _treeHead;


    int _treeHeight;

    template<class E> friend class BTree;

    /*
    end of the class BTreeImpl
    */
};

template<class T>
BTreeImpl<T>::BTreeImpl()
:_treeHeight(0),_treeHead(NULL)
{
    /*
    create an object without parameters,it will
    be called to create a null BTree.
    it won't create a new DataNode<T> * object,
    and will do that when first insert element.
    */
}

template<class T>
BTreeImpl<T>::BTreeImpl(T const& t)
:_treeHeight(1),_treeHead(new DataNode<T>())
{
    /*
    create an object by element T & t,it will be
    called to create a new BTree,and get a new 
    DataNode<T> object as the root of the tree.
    -----------------------------------------------
    insert element t into the tree,set tree height 1
    */
    insertElem(t,_treeHead);

}

template<class T>
BTreeImpl<T>::BTreeImpl(BTreeImpl<T> const & cpy)
{

}

template<class T>
BTreeImpl<T>::~BTreeImpl()
{
    /*
    each datanode in the BTree is create by operator
    new,so we should release these memory when delete
    the BTree.
    */
    _release(_treeHead);

    /*
    end of the function ~BTreeImpl()
    */
}

template<class T>
int BTreeImpl<T>::getHeight() const
{
    return _treeHeight;
}

template<class T>
DataNode<T> * BTreeImpl<T>::getRoot() const
{
    return _treeHead;
}

template<class T>
void BTreeImpl<T>::insertElem(T const& t)
{
    if(_treeHead == NULL) {
        _treeHead = new DataNode<T>();
        _updateHeight(1);
    }
    if(_insertElem(t,_treeHead)) {
        _updateHeight(1);
        _updateTHead();
    }
    /*
    end of the function insertElem()
    */
}

template<class T>
void BTreeImpl<T>::deleteElem(T const& t)
{
    if(_treeHead == NULL) return;
    if(_deleteElem(t,_treeHead)) {
        _updateHeight(-1);
        _updateTHead();
    }
}

template<class T>
void BTreeImpl<T>::printTree() const
{
    _printBTree(_treeHead);
}

template<class T>
int BTreeImpl<T>::countElems() const
{
    return __count__(_treeHead);
}

template<class T>
void BTreeImpl<T>::clearTree()
{
    if(_treeHeight) _release(_treeHead);
}

template<class T>
T const * BTreeImpl<T>::searchElem(T const& t) const
{
    return _search(t,_treeHead);    
}

/*
the functions below are some auxiliary functions,they
are just used in functions above.an we don't plan to
use them in other places.
======================================================
*/

template<class T>
int BTreeImpl<T>::_updateHeight(int val)
{

    return (_treeHeight += val);
}

template<class T>
DataNode<T> * BTreeImpl<T>::_updateTHead()
{
    _treeHead = _treeHead->_treeHead;
    return _treeHead;
    
    /*
    end of the function updateTHead()
    */
}
template<class T>
int BTreeImpl<T>::_updateHeight(int & pos,DataNode<T> * head)
{
    if(head->_numDeepth > pos) pos = head->_numDeepth;

    for(DataNode<T> * chld : head->_child)
        _updateHeight(pos,chld);

    return pos;

    /*
    end of the function _updateHeight(int,DataNode<T>*)
    */
}

template<class T>
void BTreeImpl<T>::_printBTree(DataNode<T> * head) const
{
    if(head == NULL) {
        std::cout << "NULL Tree\n";
        return;
    }

    std::cout << "[BTree Node]\n";
    std::cout << "\tDeepth - " << head->_numDeepth << "\n";
    std::cout << "\tNumcld - " << head->_numChild  << "\n";
    std::cout << "\tNumelm - " << head->_numElems  << "\n";
    std::cout << "\t[Data]\n";
    for(T const& t : head->_elems)
        std::cout << t << ":";
    std::cout << std::endl;

    for(DataNode<T> * chld : head->_child)
        _printBTree(chld);
}

template<class T>
void BTreeImpl<T>::_incHeight(DataNode<T> * head,int flag)
{
    if(flag > 0)++(head->_numDeepth);
    else --(head->_numDeepth);
    for(DataNode<T> * chld : head->_child)
        _incHeight(chld,flag);
}

template<class T>
void BTreeImpl<T>::_setRoot(
    DataNode<T> * head,DataNode<T> * root)
{
    head->_treeHead = root;
    for(DataNode<T> * chld : head->_child)
        _setRoot(chld,root);
}

template<class T>
bool BTreeImpl<T>::_insertElem(T const& t,DataNode<T> * head)
{

//#define USEITOR

#ifndef USEITOR

    int i;

    for(i = 0;i < head->_numElems;++i)
        if(head->_elems[i] >= t) break;

    if(!(head->__ISLEAF__)) 
        return _insertElem(t,head->_child[i]);

    head->_elems.insert(head->_elems.begin() + i,t);

#else

    typename std::vector<T>::iterator \
    elem_beg,elem_end;
    typename std::vector<DataNode<T>*>::iterator \
    chld_beg,chld_end;

    elem_beg = head->_elems.begin();
    elem_end = head->_elems.end();

    chld_beg = head->_child.begin();

    for(;elem_beg != elem_end;++elem_beg,++chld_beg)
        if(*elem_beg >= t) break;
    
    if(!(head->__ISLEAF__)) return _insertElem(t,*chld_beg);

    head->_elems.insert(elem_beg,t);

#endif

    if(++(head->_numElems) <= 2 * head->_numDeepth)
        return false;

    /*
    num of elems is over the limit,this node need to devide
    or tranfer one elem to one of brothers.
    */
    return _moveToBros(head) ? false : _devideBack(head);

    /*
    end of the function _insertElem(T const&,DataNode<T> *)
    */
}

template<class T>
bool BTreeImpl<T>::_deleteElem(T const& t,DataNode<T> * head)
{
    int i;

    for(i = 0;i < head->_numElems;++i) 
        if(head->_elems[i] >= t) break;

    /*
    this node is leaves
    */
    if(head->__ISLEAF__) {
        
        /*
        not such value
        */
        if(i == head->_numElems) return false;

        /*
        value found
        */
        if(head->_elems[i] == t) {

        head->_elems.erase(head->_elems.begin()+i);

        /*
        this node is also tree root,
        */
        if(head->__ISROOT__) {
            if(--(head->_numElems) == 0) return true;
            else return false;
        }

        /*
        not root and elements num is lower then numDeepth,
        */
        if(--(head->_numElems) <= 0) {
            DataNode<T> * left = head->_leftNode;
            DataNode<T> * rigt = head->_rightNode;
            DataNode<T> * tParent = head->_parent;

            /*
            find index of head in its _parent,and the index
            will be found certainly.
            */
            for(i = 0;i < tParent->_numChild;++i)
                if(tParent->_child[i] == head) break;
            
            /*
            tranfer one elem from brothers if possible.
            */
            if(left && (left->_numElems > 1)) {
            head->_elems.push_back(tParent->_elems[i-1]);
            tParent->_elems[i-1] = left->_elems[left->_numElems-1];
            left->_elems.pop_back();
            ++(head->_numElems);
            --(left->_numElems);
            return false;
            }
            if(rigt && (rigt->_numElems > 1)) {
            head->_elems.push_back(tParent->_elems[i]);
            tParent->_elems[i] = rigt->_elems[0];
            rigt->_elems.erase(rigt->_elems.begin());
            --(rigt->_numElems);
            ++(head->_numElems);
            return false;
            }

            /*
            else tranfer one node from _parent if possible.
            */
            if(tParent->_numElems > 1) {
                if(left) {
                
                left->_elems.push_back(tParent->_elems[i-1]);
                left->_rightNode = head->_rightNode;
                tParent->_elems.erase(tParent->_elems.begin()+i-1);
                tParent->_child.erase(tParent->_child.begin()+i);
                
                delete head;
                
                --(tParent->_numChild);
                --(tParent->_numElems);
                if(++(left->_numElems) > (left->_numDeepth * 2))
                    _devideBack(left);
                
                return false;
                }
                else {
                rigt->_elems.insert(rigt->_elems.begin(),
                    tParent->_elems[i]);
                rigt->_leftNode = head->_leftNode;
                tParent->_elems.erase(tParent->_elems.begin()+i);
                tParent->_child.erase(tParent->_child.begin()+i);

                delete head;

                --(tParent->_numChild);
                --(tParent->_numElems);
                if(++(rigt->_numElems) > (rigt->_numDeepth * 2))
                    _devideBack(rigt);

                return false;
                }

            }

            /*
            otherwise,parent has one element,
            */
            if(left) {
            left->_elems.push_back(tParent->_elems[0]);
            ++(left->_numElems);
            left->_rightNode = head->_rightNode;

            delete head;

            if(tParent->__ISROOT__) {
                left->__ISROOT__ = true;
                left->_treeHead = left;
                left->_parent = NULL;
                left->_leftNode = NULL;
                left->_rightNode = NULL;
                _treeHead = left;
                _incHeight(left,-1);
                return true;
            }

            return _combine(left);
            }
            /*
            else
            */
            rigt->_elems.insert(rigt->_elems.begin(),
            tParent->_elems[0]);
            ++(rigt->_numElems);
            rigt->_leftNode = head->_leftNode;

            delete head;

            if(tParent->__ISROOT__) {
                rigt->__ISROOT__ = true;
                rigt->_treeHead = rigt;
                rigt->_parent = NULL;
                rigt->_leftNode = NULL;
                rigt->_rightNode = NULL;
                _treeHead = rigt;
                _incHeight(rigt,-1);
                return true;
            }
            return _combine(rigt);
        } 

        /*
        leaves and not root and numElems > 1,delete
        */
        return false;

        }
        /*
        leaves and not such value in theis tree
        */
        return false;
    }

    /*
    this node is not leaves
    */
    if(i == head->_numElems) 
        return _deleteElem(t,head->_child[i]);

    if(t == head->_elems[i]) {
        DataNode<T> * childi = head->_child[i];
        head->_elems[i] = *(childi->_elems.end() - 1);
        return _deleteElem(head->_elems[i],childi);
    }
    
    return _deleteElem(t,head->_child[i]);

    /*
    end of the function _deleteElem()
    */
}

template<class T>
bool BTreeImpl<T>::_devideBack(DataNode<T> * head)
{
    /*
    
    */
    T temp;
    DataNode<T> * _node;

    DataNode<T> * tParent;

    tParent = head->_parent;

    temp = head->_elems[head->_numDeepth];
    /*
    devide _elems and _child from index _numDeepth(helf
    of vector size).
    */
    _node = new DataNode<T>(head,head->_numDeepth+1);

    head->_elems.erase(head->_elems.begin() \
        + head->_numDeepth,head->_elems.end());
    head->_numElems = head->_numDeepth;

    if(!(head->__ISLEAF__)) {
        head->_child.erase(head->_child.begin() \
        + head->_numDeepth + 1,head->_child.end());
        head->_numChild = head->_numDeepth + 1;
    }

    /*
    set this ndoe's right brother _node(new create).
    */
    head->_rightNode = _node;

    /*
    if this node is the root of the tree,then need a
    new tree root,and return true as thw tree growed.
    */
    if(head->__ISROOT__) {
        DataNode<T> * nRoot = new DataNode<T>(temp);

        nRoot->_child.push_back(head);
        nRoot->_child.push_back(_node);

        head->_parent = nRoot;
        _node->_parent = nRoot;
        head->__ISROOT__ = false;
        _node->__ISROOT__ = false;

        nRoot->__ISLEAF__ = false;
        nRoot->_numChild = 2;

        _incHeight(head,1);
        _incHeight(_node,1);

        _setRoot(head,nRoot);
        _setRoot(_node,nRoot);

        return true;
    }

    /*
    else insert middle value temp into _parent
    */

#ifndef USEITOR

    int i;
    for(i = 0;i < tParent->_numChild;++i)
        if(tParent->_child[i] == head) break;
    
    tParent->_elems.insert(\
    tParent->_elems.begin() + i,temp);
    tParent->_child.insert(\
    tParent->_child.begin() + i + 1,_node);

    ++(tParent->_numChild);

    if(++(tParent->_numElems) > 2 * tParent->_numDeepth)
        return _devideBack(tParent);
    
    return false;


#else 

    typename std::vector<DataNode<T>*>::iterator \
    chld_beg,chld_end;
    typename std::vector<T>::iterator \
    elem_beg,elem_end;

    chld_beg = tParent->_child.begin();
    chld_end = tParent->_child.end();

    elem_beg = tParent->_elems.begin();

    for(;chld_beg != chld_end;++chld_beg,++elem_beg)
        if(*chld_beg == head) break;

    tParent->_elems.insert(elem_beg,temp);
    tParent->_child.insert(chld_beg + 1,_node);

    ++(tParent->_numChild);

    if(++(tParent->_numElems) > 2 * tParent->_numDeepth)
        return _devideBack(tParent);
    
    return false;


#endif

    /*
    end of the function _devideBack()
    */
}

template<class T>
bool BTreeImpl<T>::_moveToBros(DataNode<T> * head)
{
    int i;
    DataNode<T> * left = head->_leftNode;
    DataNode<T> * rigt = head->_rightNode;
    DataNode<T> * tParent = head->_parent;

    if(tParent == NULL) return false;

    for(i = 0;i < tParent->_numChild;++i) 
        if(tParent->_child[i] == head) break;

    if(left && (left->_parent == head->_parent) 
    && (left->_numElems < (left->_numDeepth * 2))) {
        left->_elems.push_back(tParent->_elems[i - 1]);
        tParent->_elems[i - 1] = head->_elems[0];
        head->_elems.erase(head->_elems.begin());

        ++(left->_numElems);
        --(head->_numElems);

        return true;
    }
    if(rigt && (rigt->_parent == head->_parent) 
    && (rigt->_numElems < (rigt->_numDeepth * 2))) {
        rigt->_elems.insert(rigt->_elems.begin(),
            tParent->_elems[i]);
        tParent->_elems[i] = *(head->_elems.end());
        head->_elems.pop_back();

        ++(rigt->_numElems);
        --(head->_numElems);

        return true;
    }

    return false;
    
    /*
    end of the function _moveToBros()
    */
}


template<class T>
void BTreeImpl<T>::_release(DataNode<T> *& head)
{
    if(head == NULL) return;
    
    for(DataNode<T> * chld: head->_child)
        _release(chld);

    delete head;
    head = NULL;

    /*
    end of the function release()
    */
}

template<class T>
bool BTreeImpl<T>::_combine(DataNode<T> * head)
{

    int i;
    DataNode<T> * tParent, * left, * rigt;
    tParent = head->_parent->_parent;

    if(tParent->_numElems == 1) {
        left = tParent->_child[0];
        rigt = tParent->_child[1];
        if(left == head->_parent) {
            if(rigt->_numElems > 1) {
            left->_elems[0] = tParent->_elems[0];
            left->_child[0] = head;
            left->_child[1] = rigt->_child[0];

            left->_child[1]->_parent = left;

            tParent->_elems[0] = rigt->_elems[0];

            rigt->_elems.erase(rigt->_elems.begin());
            rigt->_child.erase(rigt->_child.begin());
            --(rigt->_numElems);
            --(rigt->_numChild);

            return false;

            }
            rigt->_elems.insert(rigt->_elems.begin(),
            tParent->_elems[0]);
            rigt->_child.insert(rigt->_child.begin(),
            head);
            ++(rigt->_numElems);
            ++(rigt->_numChild);
            rigt->_leftNode = left->_leftNode;
            delete head->_parent;
            head->_parent = rigt;
            if(tParent->__ISROOT__) {
                rigt->__ISROOT__ = true;
                rigt->_parent = NULL;
                rigt->_treeHead = rigt;
                _treeHead = rigt;
                delete tParent;
                _incHeight(rigt,-1);
                return true;
            }
            return _combine(rigt);
        }
        /*
        else,head->_parent is the right node
        */
        if(left->_numElems > 1) {
            /*
            left brother's numElem > 1,tranfer from it
            */
            rigt->_elems[0] = tParent->_elems[0];
            rigt->_child[1] = head;
            rigt->_child[0] = *(left->_child.end());

            rigt->_child[0]->_parent = rigt;

            tParent->_elems[0] = *(left->_elems.end());

            left->_elems.pop_back();
            left->_child.pop_back();

            --(left->_numElems);
            --(left->_numChild);

            return false;
        }

        left->_elems.push_back(tParent->_elems[0]);
        left->_child.push_back(head);
        ++(left->_numElems);
        ++(left->_numChild);
        left->_rightNode = rigt->_rightNode;
        delete head->_parent;
        head->_parent = left;
        if(tParent->__ISROOT__) {
            left->__ISROOT__ = true;
            left->_parent = NULL;
            left->_treeHead = left;
            _treeHead = left;
            delete tParent;
            _incHeight(left,-1);
            return true;
        }
        return _combine(left);
    }

    /*
    otherwise,tParent's numElem > 1
    */

    for(i = 0;i < tParent->_numChild;++i)
        if(tParent->_child[i] == head->_parent)
            break;
        
    left = head->_parent->_leftNode;
    rigt = head->_parent->_rightNode;

    if(left) {
        if(left->_numElems > 1) {
        head->_parent->_elems[0] = tParent->_elems[i-1];
        tParent->_elems[i-1] = left->_elems[left->_numElems-1];
        head->_parent->_child[0] = left->_child[left->_numChild-1];
        head->_parent->_child[1] = head;

        head->_parent->_child[0]->_parent = head->_parent;

        head->_parent->_child[0]->_rightNode = head;
        head->_leftNode = head->_parent->_child[0];

        left->_elems.pop_back();
        left->_child.pop_back();

        --(left->_numElems);
        --(left->_numChild);

        return false;
        }

        left->_child[1]->_rightNode = head;
        head->_leftNode = left->_child[1];

        left->_elems.push_back(tParent->_elems[i-1]);
        left->_child.push_back(head);

        tParent->_elems.erase(tParent->_elems.begin()+i-1);
        tParent->_child.erase(tParent->_child.begin()+i);

        left->_rightNode = rigt;

        delete head->_parent;
        
        head->_parent = left;

        ++(left->_numElems);
        ++(left->_numChild);

        --(tParent->_numElems);
        --(tParent->_numChild);

        return false;

    }

    head->_rightNode = rigt->_child[0];
    rigt->_child[0]->_leftNode = head;
    
    if(rigt->_numElems > 1) {
        head->_parent->_elems[0] = tParent->_elems[i];
        tParent->_elems[i] = rigt->_elems[0];
        head->_parent->_child[0] = head;
        head->_parent->_child[1] = rigt->_child[0];

        rigt->_child[0]->_parent = head->_parent;

        rigt->_elems.erase(rigt->_elems.begin());
        rigt->_child.erase(rigt->_child.begin());

        --(rigt->_numElems);
        --(rigt->_numChild);

        return false;
    }

    rigt->_elems.insert(rigt->_elems.begin(),tParent->_elems[i]);
    rigt->_child.insert(rigt->_child.begin(),head);

    tParent->_elems.erase(tParent->_elems.begin() + i);
    tParent->_child.erase(tParent->_child.begin() + i);

    rigt->_leftNode = left;

    delete head->_parent;
    
    head->_parent = rigt;

    ++(rigt->_numElems);
    ++(rigt->_numChild);

    --(tParent->_numElems);
    --(tParent->_numChild);

    return false;
    /*
    end of the function _combine()
    */
}

template<class T> 
T const *
BTreeImpl<T>::_search(T const& t,DataNode<T> * head) const
{
    for(int i=0;i < head->_numElems;++i) {
        if(head->_elems[i] == t)
            return &(head->_elems[i]);
        else if(head->_elems[i] > t) {
            if(head->_numChild == 0) return NULL;
            return _search(t,head->_child[i]);
        }
    }

    //
    if(head->_numChild == 0) return NULL;
    return _search(t,head->_child[head->_numElems]);

    /*
    end of the function search()
    */
}

template<class T>
int BTreeImpl<T>::__count__(DataNode<T> * head) const
{
    int count;

    if(head == NULL) return 0;

    count = head->_numElems;
    for(DataNode<T> * chld : head->_child)
        count += __count__(chld);

    return count;
}

#endif