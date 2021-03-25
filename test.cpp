#include<iostream>
#include"btree.hpp"

int main()
{
    BTree<int> btree;

    for(int i = 0;i < 20000;++i)
        btree.insertElem(i);
    
    //btree.printBTree();

    std::cout << "Tree Height : " << \
    btree.getTHeight() << std::endl;

    int const * c = btree.searchElem(20);

    std::cout << "search : " << *c << std::endl;

    std::cout << "Count Elements : " \
    << btree.countElems() << std::endl;

    for(int i = 0;i < 19979;++i)
        btree.deleteElem(i);

    btree.printBTree();

    std::cout << "Count Elements : " \
    << btree.countElems() << std::endl;

    std::cout << "Tree Height : " << \
    btree.getTHeight() << std::endl;

    for(int i = 0;i < 23000;++i)
        btree.insertElem(i);
    
    btree.printBTree();

    std::cout << "Tree Height : " << \
    btree.getTHeight() << std::endl;

    c = btree.searchElem(20);

    if(c)
    std::cout << "search : " << *c << std::endl;

    std::cout << "Count Elements : " \
    << btree.countElems() << std::endl;
    
    return 0;
}
