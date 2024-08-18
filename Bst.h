#ifndef BST_H
#define BST_H

#include <string>
#include <vector>

constexpr int PRINTSPACE{ 3 };

template<typename T>
class BinarySearchTree;

template<typename T>
class Node
{
public:
    T m_data{};
    Node* smaller{nullptr};
    Node* larger{nullptr};
    int m_height{0};
    int m_depth{0};

    Node(T data, int depth): m_data{data}, m_depth{depth}
    {
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
    }
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
    }
    friend class BinarySearchTree<T>;
};


template<typename T>
class BinarySearchTree
{
private:
    Node<T>* m_root{nullptr};
    std::vector<std::string> m_lines;

public:
    BinarySearchTree() = default;

    BinarySearchTree(std::vector<T> list)
    {
        for (auto& ele : list)
            insertNode(ele);
    }

    void insertNode(T data){ _insertNode(m_root, data, 0); };
    void deleteNodes(T data);
    bool searchNode(T data);
    void updateHeight();
    void updateDepth(){ _updateDepth(m_root, 0); };
    int getHeight(){ return m_root->m_height; };
    void printTree();

    ~BinarySearchTree(){ _deleteTree(m_root); }

private:
    void _insertNode(Node<T>*& node, T data, int depth);
    Node<T>* _deleteNodes(Node<T>* node, T data);
    void _deleteTree(Node<T>*& node);
    void _resetHeight(Node<T>*& node);
    void _updateHeight(Node<T>*& node);
    void _updateDepth(Node<T>*& node, int depth);
    void _printNode(Node<T>*& node, int x, int y);
};

#include "Bst.cpp"

#endif