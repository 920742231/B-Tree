#ifndef BTREE_HPP
#define BTREE_HPP

#include"btreeimpl.hpp"

template<class T>
class BTree {

    public:

    BTree();
    ~BTree();

    void insertElem(T const&);
    void deleteElem(T const&);
    T const * searchElem(T const&);
    void printBTree() const;
    int countElems() const;
    int getTHeight() const;
    void clearBTree();

    private:


    BTreeImpl<T> * _bTreeImpl;

};

template<class T>
BTree<T>::BTree()
{
    _bTreeImpl = new BTreeImpl<T>();
}

template<class T>
BTree<T>::~BTree()
{
    delete _bTreeImpl;
}

template<class T>
void BTree<T>::insertElem(T const& t)
{
    _bTreeImpl->insertElem(t);
}

template<class T>
void BTree<T>::deleteElem(T const & t)
{
    _bTreeImpl->deleteElem(t);
}

template<class T>
T const * BTree<T>::searchElem(T const& t)
{
    return _bTreeImpl->searchElem(t);
}

template<class T>
void BTree<T>::printBTree() const
{
    _bTreeImpl->printTree();
}

template<class T>
int BTree<T>::countElems() const
{
    return _bTreeImpl->countElems();
}
template<class T>
int BTree<T>::getTHeight() const
{
    return _bTreeImpl->getHeight();
}

template<class T>
void BTree<T>::clearBTree()
{
    _bTreeImpl->clearTree();
}

#endif