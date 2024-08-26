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
    Node* smaller{ nullptr };
    Node* larger{ nullptr };
    int m_height{0};
    int m_depth{0};
    int m_bf{0};
    std::string m_label{""};

    Node(T data, int depth): m_data{data}, m_depth{depth}
    {
        std::stringstream os{};
        os << std::setfill('0') << std::setw(PRINTSPACE) << m_data;
        os >> m_label;
    };

    T& getData() const
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

    bool isEqual(const Node& node)
    {
        return m_data == node.m_data;
    };

    bool isLessThan(const T data)
    {
        return m_data < data;
    };

    bool isLessThan(const Node& node)
    {
        return m_data < node.m_data;
    };

    T findMin()
    {
        T ans{ m_data };
        Node* ptr{ this };
        while (ptr->smaller != nullptr)
        {
            ptr = ptr->smaller;
            ans = ptr->m_data;
        }
        return ans;
    };

    T findMax()
    {
        T ans{ m_data };
        Node* ptr{ this };
        while (ptr->larger != nullptr)
        {
            ptr = ptr->larger;
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
    int m_balanceOrder{ 1 };
    bool m_debug{ false };
    std::vector<std::string> m_lines;

    // for rotation
    Node<T>* m_ptrParent{ nullptr };
    Node<T>* m_ptrRotHead{ nullptr };
    std::vector<Node<T>*> m_ptrRots{};

public:
    BinarySearchTree(bool debug=false): m_debug{debug}
    {
    }

    BinarySearchTree(int balanceOrder, bool debug=false): m_balanceOrder{balanceOrder}, m_debug{debug}
    {
    }

    BinarySearchTree(int balanceOrder, const std::vector<T> list, bool debug=false): m_balanceOrder{balanceOrder}, m_debug{debug}
    {
        for (auto& ele : list)
            insertNode(ele);
    }

    BinarySearchTree(BinarySearchTree& bst) // copy constructor
    {
        BinarySearchTree(bst.m_balanceOrder, bst._getSeqence(), bst.m_debug);
    }

    void insertNode(const T data);
    void insertNode(const std::vector<T> list);
    void deleteNodes(const T data);
    std::size_t findNodes(T data);
    void updateHeight();
    int getHeight(){ return m_root->m_height; };
    void printTree();

    ~BinarySearchTree(){ _deleteTree(m_root); }

private:
    void _insertNode(Node<T>*& node, const T data, int depth);
    Node<T>* _deleteNode(Node<T>* node, T data);
    Node<T>* _deleteNodes(Node<T>* node, T data);
    void _deleteTree(Node<T>*& node);
    bool _isUnbalanced(Node<T>* node){ return abs(node->m_bf)>=(2*m_balanceOrder); };
    void _fillRotationPointers();
    void _rotate();
    void __rotate();
    void _rotate1(Node<T>* parentNode, Node<T>* node, SenseType sense);
    int _getNodeHeight(Node<T>* node){ return (node == nullptr) ? -1 : node->m_height; };
    void _updateNodeHeight(Node<T>* node);
    void _setNodeHeight(Node<T>* node, int heightL, int heightR);
    void _updateHeight(Node<T>* node);
    void _updateDepth(Node<T>* node, int depth);
    std::vector<T> _getSequence(){ return {0}; };  // TODO: for deep copying
    void _printNode(Node<T>*& node, int x, int y);

};



#include "Bst.cpp"

#endif