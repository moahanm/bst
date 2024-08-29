#ifndef BST_H
#define BST_H

#include <iomanip>
#include <string>
#include <vector>

constexpr int PRINTSPACE{ 3 };

template<typename T>
class BinarySearchTree;

template<typename T>
class Node
{
private:
    T m_data{};
    Node* left{ nullptr };
    Node* right{ nullptr };
    int m_height{0};
    int m_depth{0};
    std::string m_label{""};

    Node(T data, int depth): m_data{data}, m_depth{depth}
    {
        std::stringstream os{};
        os << std::setfill('0') << std::setw(PRINTSPACE) << m_data;
        os >> m_label;
    };

    T& getData()
    {
        return m_data;
    }

    void setData(const T data)  // should not use "m_data =" as label needs updating
    {
        m_data = data;
        std::stringstream os{};
        os << std::setfill('0') << std::setw(PRINTSPACE) << m_data;
        os >> m_label;
    };

    bool isEqual(const T data)
    {
        return m_data == data;
    };

    bool isLessThan(const T data)
    {
        return m_data < data;
    };

    T findMin()
    {
        T ans{ m_data };
        Node* ptr{ this };
        while (ptr->left != nullptr)
        {
            ptr = ptr->left;
            ans = ptr->m_data;
        }
        return ans;
    };

    T findMax()
    {
        T ans{ m_data };
        Node* ptr{ this };
        while (ptr->right != nullptr)
        {
            ptr = ptr->right;
            ans = ptr->m_data;
        }
        return ans;
    };

    friend class BinarySearchTree<T>;
};


template<typename T>
class BinarySearchTree
{
private:

    enum SenseType
    {
        left,
        right
    };

    Node<T>* m_root{ nullptr };
    std::size_t m_maxBalanceFactor{ 2 };
    std::size_t m_rotationLength{ 3 };

    // rotation
    Node<T>* m_ptrParent{ nullptr };
    Node<T>* m_ptrRotHead{ nullptr };
    std::vector<Node<T>*> m_ptrRots{};

    // printing
    std::vector<std::string> m_lines;

public:
    BinarySearchTree() = default;
    
    BinarySearchTree(const std::size_t maxBalanceFactor): m_maxBalanceFactor{maxBalanceFactor}
    {
    }

    BinarySearchTree(const std::size_t maxBalanceFactor, const std::size_t rotationLength, const std::vector<T> list): 
    m_maxBalanceFactor{maxBalanceFactor}, 
    m_rotationLength{rotationLength}
    {
        for (auto& ele : list)
            insertNode(ele);
    }

    void insertNode(const T data);
    void insertNode(const std::vector<T> list);
    void deleteNodes(const T data);
    void setRotationLength(const std::size_t rotationLength);
    void clear(){ _deleteTree(m_root); }
    std::size_t findNodes(const T data);
    int getHeight() const { return m_root->m_height; };
    std::vector<T> getSequence();
    void printTree();

    virtual ~BinarySearchTree(){ _deleteTree(m_root); }

protected:
    void _insertNode(Node<T>*& node, const T data, int depth);
    Node<T>* _deleteNode(Node<T>* node, T data);
    Node<T>* _deleteNodes(Node<T>* node, T data);
    void _deleteTree(Node<T>*& node);
    Node<T>* _getNode(const T data);
    bool _isUnbalanced(Node<T>* node);
    void _fillRotationPointers();
    void _rotate();
    void _rotate23();
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