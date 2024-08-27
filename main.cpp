#include "Bst.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include "../Random.h"
#include <string>
#include "../Timer.h"
#include <vector>

int main()
{
    const int nNodes = 100000;
    // BinarySearchTree<int> bst{2};

    // for (int i{0}; i<nNodes; i++)
    //     bst.insertNode(Random::get(0,10*nNodes));

    // // for 1st order rotations
    // bst.insertNode({40,20,10,25,30,22,50});
    // bst.printTree();

    // bst.insertNode(2);
    // bst.insertNode(5);
    // bst.insertNode(1);
    // bst.insertNode(4);
    // bst.insertNode(5);
    // bst.insertNode(6);
    // bst.insertNode(7);
    // bst.insertNode(5);
    // bst.insertNode(4);
    // bst.insertNode(3);
    // bst.insertNode(3);


    // for delete rotation
    // bst.insertNode(0);
    // bst.insertNode(-4);
    // bst.insertNode(4);
    // bst.insertNode(-6);
    // bst.insertNode(-3);
    // bst.insertNode(6);
    // bst.insertNode(-8);


    // for 2nd order rotations
    // bst.clear();
    // bst.insertNode({90,70,50,30});
    // bst.printTree();
    // bst.insertNode(10);
    // bst.printTree();

    // bst.clear();
    // bst.insertNode({10,30,50,70});
    // bst.printTree();
    // bst.insertNode(90);
    // bst.printTree();

    // bst.clear();
    // bst.insertNode({90,70,50,30});
    // bst.printTree();
    // bst.insertNode(40);
    // bst.printTree();

    // bst.clear();
    // bst.insertNode({40,20,10,25,30,22,50,28,27});
    // bst.printTree();


    // std::cout << "NUMBER OF 5s: " << bst.findNodes(5) << '\n';

    // // std::cout << "BEFORE:\n";
    // bst.printTree();

    // bst.deleteNodes(5);

    // std::cout << "AFTER deleting 5:\n";
    // bst.printTree();


    std::vector<std::size_t> vecOrder{1,2,3,4};
    std::vector<double> vecTimeInsert{};
    std::vector<double> vecTimeInsert1{};
    std::vector<double> vecTimeFind{};
    std::vector<int> vecHeight{};
    Timer tictoc;
    int numberToFind{Random::get(0,10*nNodes)};
    for (std::size_t o{0}; o<vecOrder.size(); o++)
    {
        BinarySearchTree<int> bst{vecOrder[o]};
        tictoc.reset();
        for (int i{0}; i<nNodes; i++)
            bst.insertNode(Random::get(0,100*nNodes));
        vecTimeInsert.push_back(tictoc.elapsed());
        tictoc.reset();
        bst.insertNode(numberToFind);
        vecTimeInsert1.push_back(tictoc.elapsed());
        tictoc.reset();
        bst.findNode(numberToFind);
        vecTimeFind.push_back(tictoc.elapsed());
        vecHeight.push_back(bst.getHeight());
        bst.clear();
    }

    std::cout << "Binary Search Tree:\n";
    std::cout << "log(" << nNodes << ") = " << std::log(nNodes) << '\n';
    for (std::size_t o{0}; o<vecOrder.size(); o++)
    {
        std::cout << "ORDER: " << vecOrder[o] << " HEIGHT: " << vecHeight[o] << " INSERT: " << vecTimeInsert[o] << ", " << vecTimeInsert1[o] << "(1) FIND: " << vecTimeFind[o] << '\n';
    }
    std::cout << "Sorted list:\n";
    std::vector<int> vecList{};
    tictoc.reset();
    for (int i{0}; i<nNodes; i++)
        vecList.push_back(Random::get(0,100*nNodes));
    std::sort(vecList.begin(),vecList.end());
    double timeInsert{tictoc.elapsed()};
    tictoc.reset();
    vecList.push_back(numberToFind);
    std::sort(vecList.begin(),vecList.end());
    double timeInsert1{tictoc.elapsed()};
    tictoc.reset();
    auto it = std::lower_bound(vecList.begin(),vecList.end(),numberToFind);
    double timeFind{tictoc.elapsed()};
    std::cout << "INSERT: " << timeInsert << ", " << timeInsert1 << "(1) FIND: " << timeFind << '\n';

    return 0;
}