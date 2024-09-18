#include "Bst.h"
#include <chrono>
#include "Dictionary.h"
#include <iostream>
#include <iomanip>
#include "Point2d.h"
#include "../Random.h"
#include <string>
#include "../Timer.h"
#include <vector>

int main()
{
    {
    // 1-3 rotation test on Custom object
    BinarySearchTree<Point2d> bst{1};
    bst.insertNode(Point2d(4.,4.), "(4,4)");
    bst.insertNode(Point2d(2.,2.), "(2,2)");
    bst.insertNode(Point2d(1.,1.), "(1,1)");
    bst.insertNode(Point2d(2.5,2.5), "(2.5,2.5)");
    bst.insertNode(Point2d(3.,3.), "(3,3)");
    bst.insertNode(Point2d(2.2,2.2), "(2.2,2.2)");
    bst.insertNode(Point2d(5.,5.), "(5,5)");
    bst.printTree();
    }


    // {
    // // 1-3 rotation test
    // BinarySearchTree<int> bst{1};
    // bst.setRotationLength(3);
    // bst.insertNode({40,20,10,25,30,22,50}, {"040","020","010","025","030","0022","050"});
    // bst.printTree();
    // /* answer
    //          025

    //    020         040

    // 010   022   030   050
    // */
    // }


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

    // {
    // for 2nd order rotations
    // BinarySearchTree<int> bst{2};
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
    // }

    // {
    // 2-3 rotation test
    // BinarySearchTree<int> bst{2};
    // bst.setRotationLength(3);
    // bst.insertNode({40,20,10,25,30});
    // bst.printTree();
    // bst.insertNode({22,28});
    // bst.printTree();
    /* answer
             025         

       020         040   

    010         030
    
    then
                     025                     

          020                     040         

    010         022         030               

                         028

    */
    // }

    // {
    // 3-3 rotation test
    // BinarySearchTree<int> bst{3};
    // bst.setRotationLength(3);
    // bst.insertNode({40,20,10,25,30,22,28});
    // bst.printTree();
    /* answer
                     025                     

          020                     040         

    010         022         030               

                         028
    */
    // }

    // {
    // 3-4 rotation test
    // BinarySearchTree<int> bst{3};
    // bst.setRotationLength(4);
    // bst.insertNode({40,20,10,25,30,22,28});
    // bst.printTree();
    /* answer
             025         

       020         030   

    010   022   028   040
    
    */
    // }

    // std::cout << "NUMBER OF 5s: " << bst.findNodes(5) << '\n';

    // // std::cout << "BEFORE:\n";
    // bst.printTree();

    // bst.deleteNodes(5);

    // std::cout << "AFTER deleting 5:\n";
    // bst.printTree();

    /*
    const int nNodes{100000};
    const int Nave{100};
    Timer tictoc;
    std::vector<int> numbersToFind{};
    for (int k{0}; k<Nave; k++)
        numbersToFind.push_back(Random::get(0,10*nNodes));

    std::vector<std::size_t> vecMaxBF{1,2,3,4,5,6};
    std::vector<double> vecTimeInsert{};
    std::vector<double> vecTimeInsert1{};
    std::vector<double> vecTimeFind{};
    std::vector<int> vecHeight{};
    for (std::size_t bf{0}; bf<vecMaxBF.size(); bf++)
    {
        BinarySearchTree<int> bst{vecMaxBF[bf]};
        bst.setRotationLength(3);

        tictoc.reset();
        for (int i{0}; i<nNodes; i++)
            bst.insertNode(Random::get(0,100*nNodes));
        vecTimeInsert.push_back(tictoc.elapsed());

        tictoc.reset();
        bst.insertNode(numbersToFind);
        vecTimeInsert1.push_back(tictoc.elapsed()/Nave);

        tictoc.reset();
        for (int k{0}; k<Nave; k++)
            bst.findNodes(numbersToFind[k]);
        vecTimeFind.push_back(tictoc.elapsed()/Nave);

        vecHeight.push_back(bst.getHeight());
    }
    std::cout << "Binary Search Tree (rotation length = 3):\n";
    std::cout << "log( " << nNodes << " ) = " << std::log(nNodes) << '\n';
    for (std::size_t bf{0}; bf<vecMaxBF.size(); bf++)
    {
        std::cout << "MaxBalFac: " << vecMaxBF[bf] << " HEIGHT: " << vecHeight[bf] << " INSERT: " << vecTimeInsert[bf] << ", " << vecTimeInsert1[bf] << "(1) FIND: " << vecTimeFind[bf] << '\n';
    }

    std::cout << "\n\n";

    std::vector<std::size_t> vecRotLen{3,4,5,6,7};
    vecTimeInsert.clear();
    vecTimeInsert1.clear();
    vecTimeFind.clear();
    vecHeight.clear();
    for (std::size_t rl{0}; rl<vecRotLen.size(); rl++)
    {
        BinarySearchTree<int> bst{5};
        bst.setRotationLength(vecRotLen[rl]);

        tictoc.reset();
        for (int i{0}; i<nNodes; i++)
            bst.insertNode(Random::get(0,100*nNodes));
        vecTimeInsert.push_back(tictoc.elapsed());

        tictoc.reset();
        bst.insertNode(numbersToFind);
        vecTimeInsert1.push_back(tictoc.elapsed()/Nave);

        tictoc.reset();
        for (int k{0}; k<Nave; k++)
            bst.findNodes(numbersToFind[k]);
        vecTimeFind.push_back(tictoc.elapsed()/Nave);

        vecHeight.push_back(bst.getHeight());
    }
    std::cout << "Binary Search Tree (max balance factor = 5):\n";
    std::cout << "log( " << nNodes << " ) = " << std::log(nNodes) << '\n';
    for (std::size_t rl{0}; rl<vecRotLen.size(); rl++)
    {
        std::cout << "RotLen: " << vecRotLen[rl] << " HEIGHT: " << vecHeight[rl] << " INSERT: " << vecTimeInsert[rl] << ", " << vecTimeInsert1[rl] << "(1) FIND: " << vecTimeFind[rl] << '\n';
    }

    std::cout << "\n\n";

    std::cout << "Sorted list:\n";
    std::vector<int> vecList{};
    
    tictoc.reset();
    for (int i{0}; i<nNodes; i++)
        vecList.push_back(Random::get(0,100*nNodes));
    std::sort(vecList.begin(),vecList.end());
    double timeInsert{tictoc.elapsed()};

    tictoc.reset();
    for (int k{0}; k<Nave; k++)
    {
        vecList.push_back(numbersToFind[k]);
        std::sort(vecList.begin(),vecList.end());
    }
    double timeInsert1{tictoc.elapsed()/Nave};

    tictoc.reset();
    for (int k{0}; k<Nave; k++)
        auto it = std::lower_bound(vecList.begin(),vecList.end(),numbersToFind[k]);
    double timeFind{tictoc.elapsed()/Nave};

    std::cout << "INSERT: " << timeInsert << ", " << timeInsert1 << "(1) FIND: " << timeFind << '\n';

    */
    return 0;
}