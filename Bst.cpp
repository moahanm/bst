#include "Bst.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


void BinarySearchTree::_insertNode(Node*& node, int data, int depth)
{
    if (node != nullptr)
    {
        if (data < node->m_data)
        {
            _insertNode(node->smaller, data, depth+1);
        }
        else
            _insertNode(node->larger, data, depth+1);        
    }
    else
    {
        node = new Node{data, depth};
    }
    updateHeight();
}

// Delete all nodes with data 'data'
void BinarySearchTree::deleteNodes(int data)
{
    m_root = _deleteNodes(m_root, data);
    updateHeight();
    updateDepth();
}

Node* BinarySearchTree::_deleteNodes(Node* node, int data)
{
    if (node != nullptr)
    {
        if (node->m_data == data)
        {
            if (node->m_height == 0)
            {
                delete node;
                node = nullptr;
            }
            else if (node->smaller != nullptr && node->larger == nullptr)
            {
                Node* ptr{ node };
                node = node->smaller;
                delete ptr;
            }
            else if (node->smaller == nullptr && node->larger != nullptr)
            {
                Node* ptr{ node };
                node = node->larger;
                delete ptr;
            }
            else
            {
                node->m_data = node->smaller->findMax(); // no duplicates guaranteed in smaller subtree
                node->smaller = _deleteNodes(node->smaller, node->m_data);
            }
        }
        if (node != nullptr)    // for leaf case
        {
            node->smaller = _deleteNodes(node->smaller, data);
            node->larger = _deleteNodes(node->larger, data);
        }

        return node;
    }
    else
        return nullptr;
}

int BinarySearchTree::_deleteTree(Node*& node)
{
    int ans{0};
    if (node != nullptr)
    {
        ans = node->m_data;
        _deleteTree(node->smaller);
        _deleteTree(node->larger);
        delete node;
        node = nullptr;
    }
    return ans;
}

bool BinarySearchTree::searchNode(int data)
{
    if (m_root != nullptr)
    {
        Node* node{ m_root };

        while (true)
        {
            if (data == node->m_data)
            {
                return true;
            }
            else if (data < node->m_data)
            {
                if (node->smaller != nullptr)
                {
                    node = node->smaller;
                }
                else
                    return false;
            
            }
            else if (data > node->m_data)
            {
                if (node->larger != nullptr)
                {
                    node = node->larger;
                }
                else
                    return false;
            }
        }
    }
    else
        return false;
}

void BinarySearchTree::updateHeight()
{
    _resetHeight(m_root);
    _updateHeight(m_root);
}

// Reset height values for all nodes with root 'node' inclusive
void BinarySearchTree::_resetHeight(Node*& node)
{
    if (node != nullptr)
    {
        node->m_height = 0;
        _resetHeight(node->smaller);
        _resetHeight(node->larger);
    }
}

// Update height values for all nodes with root 'node' inclusive
void BinarySearchTree::_updateHeight(Node*& node)
{
    if (node != nullptr)
    {
        _updateHeight(node->smaller);
        _updateHeight(node->larger);

        if (node->smaller == nullptr && node->larger == nullptr)    // leaf
        {
            node->m_height = 0;
        }
        else if (node->smaller == nullptr && node->larger != nullptr)
        {
            node->m_height = node->larger->m_height + 1;
        }
        else if (node->smaller != nullptr && node->larger == nullptr)
        {
            node->m_height = node->smaller->m_height + 1;
        }
        else
            node->m_height = std::max(node->smaller->m_height,node->larger->m_height) + 1;
    }
}

void BinarySearchTree::_updateDepth(Node*& node, int depth)
{
    if (node != nullptr)
    {
        node->m_depth = depth;
        _updateDepth(node->smaller, depth+1);
        _updateDepth(node->larger, depth+1);
    }
}

void BinarySearchTree::printTree()
{
    if (m_root != nullptr)
    {
        updateHeight();

        int nlines{ m_root->m_height + 1 };
        int pWidth{2};
        for (int i{0}; i<nlines-1; i++)
            pWidth *= 2;
        pWidth -= 1;

        // Recreate canvas
        std::string line{""};
        for (int w{0}; w<pWidth; w++)
            for (int i{0}; i<PRINTSPACE; i++)
                line += ' ';
        m_lines.clear();
        for (int h{0}; h<nlines; h++)
            m_lines.push_back(line);

        std::string& line2{ m_lines[0] };

        // Printing
        _printNode(m_root, (pWidth-1)/2, 0);
        for (int h{0}; h<nlines; h++)
            std::cout << m_lines[h] << "\n\n";
    }
    else
    {
        std::cout << "Null tree\n";
    }
}

void BinarySearchTree::_printNode(Node*& node, int x, int y)
{
    if (node != nullptr)
    {
        std::string& line{ m_lines[y] };
        std::stringstream os{};
        os << std::setfill('0') << std::setw(PRINTSPACE) << node->m_data;
        std::string label{};
        os >> label;
        for (int i{0}; i<PRINTSPACE; i++)
            line[x*PRINTSPACE+i] = label[i];

        int H{m_root->m_height};
        int d{node->m_depth};
        int shift{1};
        for (int i{0}; i<H-d-1; i++)
            shift *= 2;

        _printNode(node->smaller, x-shift, y+1);
        _printNode(node->larger, x+shift, y+1);
    }
}