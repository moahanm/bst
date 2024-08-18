#ifndef BST_H
#define BST_H

#include <string>
#include <vector>

constexpr int PRINTSPACE{ 3 };

class Node
{
private:
    int m_data{};
    Node* smaller{nullptr};
    Node* larger{nullptr};
    int m_height{0};
    int m_depth{0};

    Node(int data, int depth): m_data{data}, m_depth{depth}
    {
    };

    int findMin()
    {
        int ans{ m_data };
        Node* ptr{ this };
        while (ptr->smaller != nullptr)
        {
            ptr = ptr->smaller;
            ans = ptr->m_data;
        }
        return ans;
    }
    int findMax()
    {
        int ans{ m_data };
        Node* ptr{ this };
        while (ptr->larger != nullptr)
        {
            ptr = ptr->larger;
            ans = ptr->m_data;
        }
        return ans;
    }
    friend class BinarySearchTree;
};

class BinarySearchTree
{
private:
    Node* m_root{nullptr};
    std::vector<std::string> m_lines;

public:
    BinarySearchTree() = default;

    BinarySearchTree(std::vector<int> list)
    {
        for (auto& ele : list)
            insertNode(ele);
    }

    void insertNode(int data){ _insertNode(m_root, data, 0); };
    void deleteNodes(int data);
    bool searchNode(int data);
    void updateHeight();
    void updateDepth(){ _updateDepth(m_root, 0); };
    int getHeight(){ return m_root->m_height; };
    void printTree();

    ~BinarySearchTree(){ _deleteTree(m_root); }

private:
    void _insertNode(Node*& node, int data, int depth);
    Node* _deleteNodes(Node* node, int data);
    int _deleteTree(Node*& node);
    void _resetHeight(Node*& node);
    void _updateHeight(Node*& node);
    void _updateDepth(Node*& node, int depth);
    void _printNode(Node*& node, int x, int y);
};


#endif