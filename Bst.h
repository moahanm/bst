#ifndef BST_H
#define BST_H

#include "DLList.h"
#include <string>
#include <vector>

/*
BinarySearchTree is a generalisation of the AVL binary search tree, with custom
- maximum balance factor (e.g. 1 for AVL)
- rotation length (e.g. 3 for AVL)

Example #1: m_maxBalanceFactor = 1

m_rotationLength = 3

        A                   B                       A                       C
    B           =>      C       A       or      B               =>      B       A
  C                                               C

Example #2: m_maxBalanceFactor = 2

m_rotationLength = 3                                m_rotationLength = 5

                A                       B                               A                       E
            B           =>          C       A                       B           =>          D       B
        C                             D                         C                         C           A
          D                            E                          D
           E                                                       E

*/

template<typename T>
class BinarySearchTree
{
using Element = typename DLList<T>::Element;
using Iterator = typename DLList<T>::Iterator;

protected:
    class Node
    {
    public:
        Element* m_elementPointer;   // no default constructor assumed
        Node* left{ nullptr };
        Node* right{ nullptr };
        int m_height{ 0 };  // for balance factor calculation
        int m_depth{ 0 };   // for printing
        std::string m_label{ "" };    // for printing

        Node(Element* elementPointer, int depth, std::string label=""):
        m_elementPointer{ elementPointer }, m_depth{ depth }, m_label{ label }
        {
        }

        Node() = delete;

        void setLabel(std::string label){ m_label = label; }

        bool operator==(const T& data) const { return *(m_elementPointer->dataPointer) == data; }

        bool operator<(const T& data) const { return *(m_elementPointer->dataPointer) < data; }

        virtual ~Node()
        {
        }
    };

    enum SenseType
    {
        left,
        right
    };

    DLList<T> m_elements;

    Node* m_root{ nullptr };
    std::size_t m_maxBalanceFactor{ 1 };
    std::size_t m_rotationLength{ 3 };

    // rotation
    Node* m_ptrParent{ nullptr };
    Node* m_ptrRotHead{ nullptr };
    std::vector<Node*> m_ptrRots{};

    // printing
    std::size_t printSpace{ 1 };
    std::vector<std::string> m_lines;

public:
    BinarySearchTree() = default;

    BinarySearchTree(std::size_t maxBalanceFactor): m_maxBalanceFactor{maxBalanceFactor}
    {
    }

    BinarySearchTree(std::size_t maxBalanceFactor, std::size_t rotationLength, const std::vector<T>& list, std::vector<std::string> labels = std::vector<std::string>()): 
    m_maxBalanceFactor{maxBalanceFactor}, 
    m_rotationLength{rotationLength}
    {
        for (std::size_t i{0}; i<list.size(); i++)
        {
            if (labels.size() == 0)
            {
                insertNode(list[i]);
            }
            else
                insertNode(list[i], labels[i]);
        }
    }

    void insertNode(const T& data, std::string label="");

    void insertNode(const std::vector<T>& list, std::vector<std::string> labels=std::vector<std::string>());

    void deleteNode(const T& data);

    void setRotationLength(const std::size_t rotationLength);

    Iterator begin(){ return m_elements.begin(); }

    Iterator end(){ return m_elements.end(); }

    void clear(){ _deleteTree(m_root); }

    std::size_t findNodes(const T& data) const;
    
    int getHeight() const { return m_root->m_height; }

    std::vector<T> getSequence();

    void printTree();

    virtual ~BinarySearchTree(){ _deleteTree(m_root); }

protected:
    void _insertNode(Node*& node, const T& data, int depth, std::string label);

    Node* _createNode(const T& data, int depth, std::string label);

    Node* _deleteNode(Node* node, const T& data);

    Node* _deleteNodes(Node* node, const T& data);

    void _deleteTree(Node*& node);

    void _copyNode(Node* nodeReference, Node* nodeTarget);

    void _freeNode(Node*& node);

    bool _isUnbalanced(Node* node);

    void _fillRotationPointers();

    void _rotate();

    void _rotateAVL();

    void _rotate1(Node* parentNode, Node* node, SenseType sense);

    Node* _findMinFrom(Node *node);

    Node* _findMaxFrom(Node *node);

    int _getNodeHeight(Node* node){ return (node == nullptr) ? -1 : node->m_height; };

    int _getBalanceFactor(Node* node){ return _getNodeHeight(node->left)-_getNodeHeight(node->right); };

    void _updateNodeHeight(Node* node);

    void _updateDepth(Node* node, int depth);

    void _printNode(Node*& node, int x, int y);

};

#include "Bst.cpp"

#endif