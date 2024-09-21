#include "Bst.h"
#include <chrono>
#include "Dictionary.h"
#include <iostream>
#include <iomanip>
#include "Point2d.h"
#include "Random.h"
#include <string>
#include "Timer.h"
#include <vector>

/*
To compile:

g++ main.cpp -o bst

*/

int main()
{
    // {
    // std::cout << "1-3 rotation test (AVL):\n";
    // BinarySearchTree<int> bst{1};
    // bst.setRotationLength(3);
    // bst.insertNode({40,20,10,25,30,22,50}, {"040","020","010","025","030","022","050"});
    // bst.printTree();
    // /* answer
    //          025

    //    020         040

    // 010   022   030   050
    // */
    // }

    // {
    // std::cout << "1-3 rotation test on Custom object:\n";
    // BinarySearchTree<Point2d> bst{};
    // bst.insertNode(Point2d(4.,4.), "(4,4)");
    // bst.insertNode(Point2d(2.,2.), "(2,2)");
    // bst.insertNode(Point2d(1.,1.), "(1,1)");
    // bst.insertNode(Point2d(2.5,2.5), "(2.5,2.5)");
    // bst.insertNode(Point2d(3.,3.), "(3,3)");
    // bst.insertNode(Point2d(2.2,2.2), "(2.2,2.2)");
    // bst.insertNode(Point2d(5.,5.), "(5,5)");
    // bst.printTree();
    // }

    {
    std::cout <<  "1-3 deletion test (AVL):\n";
    BinarySearchTree<int> bst{1};
    bst.insertNode({2,5,1,4,5,6,7,5,4,3,3}, {"2","5","1","4","5","6","7","5","4","3","3"});
    std::cout << "Before:\n";
    bst.printTree();
    bst.deleteNode(5);
    std::cout << "After deletion (5):\n";
    bst.printTree();
    /* answer
           5       

       3       6   

     2   4   5   7 

    1   3 4   5

    then
           4       

       3       6   

     2   3       7 

    1     4
    */
    }

    // {
    // std::cout << "2-3 rotation tests:\n";
    // BinarySearchTree<int> bst{2};
    // bst.insertNode({90,70,50,30},{"90","70","50","30"});
    // bst.insertNode(10,"10");
    // bst.printTree();
    // /* answer
    //               70              

    //       50              90      

    //   30                          

    // 10
    // */

    // bst.clear();
    // bst.insertNode({40,20,10,25,30,22,50,28,27},{"40","20","10","25","30","22","50","28","27"});
    // bst.printTree();
    // /* answer
    //                               25                              

    //               20                              40              

    //       10              22              30              50      

    //                                    28                          

    //                                  27
    // */
    // }

    {
    std::cout << "2-5 rotation tests:\n";
    BinarySearchTree<int> bst{2};
    bst.setRotationLength(5);
    bst.insertNode({90,70,50,30,10},{"90","70","50","30","10"});
    bst.printTree();
    /* answer
            50      

        30      70  

      10          90
    */
    }

    // {
    // std::cout << "3-3 rotation test:\n";
    // BinarySearchTree<int> bst{3};
    // bst.insertNode({40,20,10,25,30,22,28}, {"040","020","010","025","030","022","028"});
    // bst.printTree();
    // /* answer
    //                  025                     

    //       020                     040         

    // 010         022         030               

    //                      028
    // */
    // }

    // {
    // std::cout << "3-4 rotation test:\n";
    // BinarySearchTree<int> bst{3};
    // bst.setRotationLength(4);
    // bst.insertNode({40,20,10,25,30,22,28}, {"040","020","010","025","030","022","028"});
    // bst.printTree();
    // /* answer
    //          025         

    //    020         030   

    // 010   022   028   040
    
    // */
    // }

    
    
    const int nNodes{100000};
    const int Nave{100};

    std::cout << "\nSpeed-test for insertion and finding with " << nNodes << " integers with " << Nave << " values for time averaging in seconds.\n\n";
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
        std::cout << "MaxBalFac: " << vecMaxBF[bf] << " HEIGHT: " << vecHeight[bf] << " INSERT: " << vecTimeInsert[bf] << ", " << vecTimeInsert1[bf] << "(1) FIND: " << vecTimeFind[bf] << "(1)";
        if (vecMaxBF[bf] == 1)
            std::cout << " (AVL)";
        std::cout << '\n';
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
        std::cout << "RotLen: " << vecRotLen[rl] << " HEIGHT: " << vecHeight[rl] << " INSERT: " << vecTimeInsert[rl] << ", " << vecTimeInsert1[rl] << "(1) FIND: " << vecTimeFind[rl] << "(1) \n";
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

    std::cout << "INSERT: " << timeInsert << ", " << timeInsert1 << "(1) FIND: " << timeFind << "(1) \n";
    
    std::cout << "\n\n";
    

    {
    std::cout << "Using BST as a dictionary:\n";
    Dictionary<std::string,Point2d> dict{};
    dict.emplace("Alice", Point2d(3.,3.));
    dict.emplace("Bob", Point2d(2.,2.));
    dict.emplace("Charlie", Point2d(1.,1.));
    dict.emplace("Dan", Point2d(5.,5.));
    dict["Alice"]++;
    dict["Bob"] = Point2d(2.5,2.5);
    dict.emplace("Charlie", Point2d(0.,0.));
    dict.erase("Dan");
    for (const auto it : dict)
        std::cout << it.first << ", " << it.second << '\n';

    std::cout << dict.contains("Dan") << '\n';
    dict.printTree();
    }
    
    return 0;
}