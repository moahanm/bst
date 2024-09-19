#ifndef BST_H
#define BST_H

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
class BinarySearchTree;


template<typename T>
class Node
{
public:
    T m_data;   // no default constructor assumed
    Node* left{ nullptr };
    Node* right{ nullptr };
    int m_height{0};
    int m_depth{0};
    std::string m_label{""};    // for printing


private:
    Node(const T& data, int depth, std::string label=""): m_data{data}, m_depth{depth}, m_label{label}
    {
    }

    Node() = delete;

public:
    void copyNode(const Node* node)
    {
        m_data = node->m_data;
        m_label = node->m_label;
    }

    void setLabel(std::string label){ m_label = label; }

    Node* findMin()
    {
        Node* ptr{ this };
        while (ptr->left != nullptr)
            ptr = ptr->left;
        return ptr;
    }

    Node* findMax()
    {
        Node* ptr{ this };
        while (ptr->right != nullptr)
            ptr = ptr->right;
        return ptr;
    }

    bool operator==(const T& data) const { return m_data == data; }

    bool operator<(const T& data) const { return m_data < data; }

    virtual ~Node()
    {
    }

    friend class BinarySearchTree<T>;
};


template<typename T>
class BinarySearchTree
{
protected:
    enum SenseType
    {
        left,
        right
    };

    Node<T>* m_root{ nullptr };
    std::size_t m_maxBalanceFactor{ 1 };
    std::size_t m_rotationLength{ 3 };

    // rotation
    Node<T>* m_ptrParent{ nullptr };
    Node<T>* m_ptrRotHead{ nullptr };
    std::vector<Node<T>*> m_ptrRots{};

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

    void clear(){ _deleteTree(m_root); }

    std::size_t findNodes(const T& data);

    int getHeight() const { return m_root->m_height; };
    std::vector<T> getSequence();

    void printTree();

    virtual ~BinarySearchTree(){ _deleteTree(m_root); }

protected:
    void _insertNode(Node<T>*& node, const T& data, int depth, std::string label);

    Node<T>* _deleteNode(Node<T>* node, const T& data);

    Node<T>* _deleteNodes(Node<T>* node, const T& data);

    void _deleteTree(Node<T>*& node);

    bool _isUnbalanced(Node<T>* node);

    void _fillRotationPointers();

    void _rotate();

    void _rotateAVL();

    void _rotate1(Node<T>* parentNode, Node<T>* node, SenseType sense);

    int _getNodeHeight(Node<T>* node){ return (node == nullptr) ? -1 : node->m_height; };

    int _getBalanceFactor(Node<T>* node){ return _getNodeHeight(node->left)-_getNodeHeight(node->right); };

    void _updateNodeHeight(Node<T>* node);

    void _setNodeHeight(Node<T>* node, int heightL, int heightR);

    void _updateHeight(Node<T>* node);

    void _updateDepth(Node<T>* node, int depth);

    void _printNode(Node<T>*& node, int x, int y);

};

#include "Bst.cpp"

#endif