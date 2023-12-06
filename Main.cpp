#include <iostream>
#include "Interface.h"
#include "Tree.h"
#include "SmartPointer.h"

int main()
{
    interface<int>::run();
    // Pointer testing
    SmartPointer<CTree<int>> treePointer(new CTree<int>());
    SmartPointer<CTree<int>> secondPointer(new CTree<int>());
    treePointer = secondPointer;


    return 0;
}
