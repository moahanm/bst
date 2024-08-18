#include "Bst.h"
#include <chrono>
#include <iostream>
#include "../Random.h"
#include <string>
#include <vector>

int main()
{
    const int nNodes = 10;
    BinarySearchTree bst{};

    // for (int i{0}; i<nNodes; i++)
    //     bst.insertNode(Random::get(0,10*nNodes));
    bst.insertNode(2);
    bst.insertNode(5);
    bst.insertNode(1);
    bst.insertNode(4);
    bst.insertNode(5);
    bst.insertNode(6);
    bst.insertNode(7);
    bst.insertNode(5);

    std::cout << "BEFORE:\n";
    bst.printTree();

    bst.deleteNodes(5);

    std::cout << "AFTER deleting 5:\n";
    bst.printTree();

    return 0;
}