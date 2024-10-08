#include "Bst.h" // dummy
#include <algorithm>
#include <cmath>
#include "DLList.h"
#include <exception>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>


template<typename T>
void BinarySearchTree<T>::insertNode(const T& data, std::string label)
{
    _insertNode(m_root, data, 0, label);
    // std::cout << "INSERTED " << data << '\n';
    _rotate();
}

template<typename T>
void BinarySearchTree<T>::insertNode(const std::vector<T>& list, std::vector<std::string> labels)
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


template<typename T>
void BinarySearchTree<T>::_insertNode(Node*& node, const T& data, int depth, std::string label)
{
    if (node != nullptr)
    {
        if (*(node->m_elementPointer->dataPointer) < data || *(node->m_elementPointer->dataPointer) == data)
        {
            _insertNode(node->right, data, depth+1, label);
        }
        else
            _insertNode(node->left, data, depth+1, label);

        int lh = _getNodeHeight(node->left);
        int rh = _getNodeHeight(node->right);
        node->m_height = std::max(lh,rh) + 1;

        if (m_ptrRotHead == nullptr)
        {
            if (_isUnbalanced(node))
                m_ptrRotHead = node;
        }
        else if (m_ptrParent == nullptr)
            m_ptrParent = node;
    }
    else
    {
        if (label.size() > printSpace)
            printSpace = label.size();
        node = _createNode(data, depth, label);
    }
}

template<>
void BinarySearchTree<std::string>::_insertNode(Node*& node, const std::string& data, int depth, std::string label)
{
    if (node != nullptr)
    {
        if (*(node->m_elementPointer->dataPointer) < data || *(node->m_elementPointer->dataPointer) == data)
        {
            _insertNode(node->right, data, depth+1, label);
        }
        else
            _insertNode(node->left, data, depth+1, label);

        int lh = _getNodeHeight(node->left);
        int rh = _getNodeHeight(node->right);
        node->m_height = std::max(lh,rh) + 1;

        if (m_ptrRotHead == nullptr)
        {
            if (_isUnbalanced(node))
                m_ptrRotHead = node;
        }
        else if (m_ptrParent == nullptr)
            m_ptrParent = node;
    }
    else
    {
        if (label.size() == 0)
            label = data;
        if (label.size() > printSpace)
            printSpace = label.size();
        node = _createNode(data, depth, label);
    }
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_createNode(const T& data, int depth, std::string label)
{
    typename DLList<T>::Element* ptr{ m_elements.push(data) };
    return new Node{ptr, depth, label};
}

// Delete all nodes with data 'data'
template<typename T>
void BinarySearchTree<T>::deleteNode(const T& data)
{
    if (m_maxBalanceFactor>0)
    {
        std::size_t n{ count(data) };
        for (std::size_t i{0}; i<n; i++)
        {
            m_root = _deleteNode(m_root, data);
            _rotate();
        }
    }
    else
        m_root = _deleteNodes(m_root, data);
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_deleteNode(Node* node, const T& data)
{
    if (node != nullptr)
    {
        if (*(node->m_elementPointer->dataPointer) == data)
        {
            if (node->m_height == 0)
            {
                _freeNode(node);
                node = nullptr;
            }
            else if (node->left != nullptr && node->right == nullptr)
            {
                Node* ptr{ node };
                node = node->left;
                _freeNode(ptr);
            }
            else if (node->left == nullptr && node->right != nullptr)
            {

                Node* ptr{ node };
                node = node->right;
                _freeNode(ptr);
            }
            else
            {
                _copyNode(_findMaxFrom(node->left), node); // no duplicates guaranteed in left subtree
                node->left = _deleteNode(node->left, *(node->m_elementPointer->dataPointer));
                // no change in depth
            }
        }
        else if (*(node->m_elementPointer->dataPointer) < data)
        {
            node->right = _deleteNode(node->right, data);
        }
        else
        {
            node->left = _deleteNode(node->left, data);
        }

        if (node != nullptr)
        {
            _updateNodeHeight(node);

            if (m_ptrRotHead == nullptr)
            {
                if (_isUnbalanced(node))
                    m_ptrRotHead = node;
            }
            else if (m_ptrParent == nullptr)
                m_ptrParent = node;
        }

        return node;
    }
    else
        return nullptr;
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_deleteNodes(Node* node, const T& data)
{
    if (node != nullptr)
    {
        if (*(node->m_elementPointer->dataPointer) == data)
        {
            if (node->m_height == 0)
            {
                _freeNode(node);
                node = nullptr;
            }
            else if (node->left != nullptr && node->right == nullptr)
            {
                Node* ptr{ node };
                node = node->left;
                _freeNode(ptr);
            }
            else if (node->left == nullptr && node->right != nullptr)
            {
                Node* ptr{ nullptr };
                while (node != nullptr && *(node->m_elementPointer->dataPointer) == data)
                {
                    ptr = node;
                    node = node->right;
                    _freeNode(ptr);
                }
            }
            else
            {
                _copyNode(_findMaxFrom(node->left), node); // no duplicates guaranteed in left subtree
                node->left = _deleteNode(node->left, *(node->m_elementPointer->dataPointer));
                // no change in depth
            }
        }

        if (node != nullptr)
        {
            node->left = _deleteNodes(node->left, data);
            node->right = _deleteNodes(node->right, data);

            _updateNodeHeight(node);
        }

        return node;
    }
    else
        return nullptr;
}

template<typename T>
void BinarySearchTree<T>::setRotationLength(const std::size_t rotationLength)
{
    if (rotationLength < 3)
        throw std::runtime_error("rotation length must be at least 3");
    
    if (rotationLength > m_maxBalanceFactor + 2)
        throw std::runtime_error("rotation length must be at most maximum balance factor + 2");

    m_rotationLength = rotationLength;
}

template<typename T>
void BinarySearchTree<T>::_deleteTree(Node*& node)
{
    if (node != nullptr)
    {
        _deleteTree(node->left);
        _deleteTree(node->right);
        _freeNode(node);
    }
}

template<typename T>
void BinarySearchTree<T>::_copyNode(Node* nodeReference, Node* nodeTarget)
{
    m_elements.remove(nodeTarget->m_elementPointer);
    nodeTarget->m_elementPointer = m_elements.push(*(nodeReference->m_elementPointer->dataPointer));
    nodeTarget->m_label = nodeReference->m_label;
}

template<typename T>
void BinarySearchTree<T>::_freeNode(Node*& node)
{
    m_elements.remove(node->m_elementPointer);
    delete node;
    node = nullptr;
}

template<typename T>
bool BinarySearchTree<T>::_isUnbalanced(Node* node)
{
    if (node != nullptr)
    {
        int bf = _getBalanceFactor(node);
        return abs(bf) > m_maxBalanceFactor;
    }
    return false;
}

template<typename T>
typename DLList<T>::Iterator BinarySearchTree<T>::find(const T& data)
{

    Node* node{ m_root };

    while (node != nullptr)
    {
        if (*(node->m_elementPointer->dataPointer) == data)
        {
            return typename DLList<T>::Iterator{node->m_elementPointer};
        }
        else if (*(node->m_elementPointer->dataPointer) < data)
        {
            node = node->right;
        }
        else
            node = node->left;
    }
    return m_elements.end();
}

template<typename T>
std::size_t BinarySearchTree<T>::count(const T& data) const
{

    std::size_t ans{0};

    Node* node{ m_root };

    while (node != nullptr)
    {
        if (*(node->m_elementPointer->dataPointer) == data)
        {
            ans++;
            node = node->right;    // duplicates may exist in right subtree
        }
        else if (*(node->m_elementPointer->dataPointer) < data)
        {
            node = node->right;
        }
        else
            node = node->left;
    }
    return ans;
}

template<typename T>
void BinarySearchTree<T>::_fillRotationPointers()
{
    m_ptrRots.clear();

    if (m_ptrRotHead != nullptr)
    {
        m_ptrRots.emplace_back(m_ptrRotHead);
        for (std::size_t i{0};i<m_rotationLength-1;i++)
        {
            if (_getBalanceFactor(m_ptrRots[i])>0)
            {
                m_ptrRots.emplace_back(m_ptrRots[i]->left);
            }
            else
                m_ptrRots.emplace_back(m_ptrRots[i]->right);
        }
    }
}

template<typename T>
void BinarySearchTree<T>::_rotate()
{
    if (m_ptrRotHead != nullptr)
    {
        // std::cout << "ROTATING AT PIVOT " << m_ptrRotHead->m_label << '\n';
        _fillRotationPointers();
        Node* ptr1{ nullptr };
        Node* ptr2{ nullptr };

        if (_getBalanceFactor(m_ptrRotHead)>0)
        {
            // straighten right-leaning chain
            for (std::size_t i{0}; i<m_rotationLength-2; i++)
            {
                ptr1 = m_ptrRotHead;
                for (std::size_t j{0}; j<i; j++)
                    ptr1 = ptr1->left;
                ptr2 = ptr1->left;
                while (ptr2->right != nullptr)
                {
                    auto it = std::find(m_ptrRots.begin(),m_ptrRots.end(), ptr2->right);
                    if (it != m_ptrRots.end())
                    {
                        _rotate1(ptr1, ptr2, SenseType::left);
                    }
                    else
                        break;
                    ptr2 = ptr1->left;
                }
            }
            // bend straight chain rightwards
            ptr2 = m_ptrRotHead;
            for (std::size_t i{0}; i<m_rotationLength/2; i++)
            {
                ptr1 = ptr2;
                ptr2 = ptr2->left;
                _rotate1(m_ptrParent, ptr1, SenseType::right);
            }
        }
        else
        {
            // straighten left-leaning chain
            for (std::size_t i{0}; i<m_rotationLength-2; i++)
            {
                ptr1 = m_ptrRotHead;
                for (std::size_t j{0}; j<i; j++)
                    ptr1 = ptr1->right;
                ptr2 = ptr1->right;
                while (ptr2->left != nullptr)
                {
                    auto it = std::find(m_ptrRots.begin(),m_ptrRots.end(), ptr2->left);
                    if (it != m_ptrRots.end())
                    {
                        _rotate1(ptr1, ptr2, SenseType::right);
                    }
                    else
                        break;
                    ptr2 = ptr1->right;
                }
            }
            // bend straight chain leftwards
            ptr2 = m_ptrRotHead;
            for (std::size_t i{0}; i<m_rotationLength/2; i++)
            {
                ptr1 = ptr2;
                ptr2 = ptr2->right;
                _rotate1(m_ptrParent, ptr1, SenseType::left);
            }
        }
    }
    m_ptrParent = nullptr;
    m_ptrRotHead = nullptr;
}

// Just for reference
template<typename T>
void BinarySearchTree<T>::_rotateAVL()
{
    if (m_ptrRotHead != nullptr)
    {
        // std::cout << "ROTATING AT PIVOT " << m_ptrRotHead->m_label << '\n';
        _fillRotationPointers();
        Node* ptr1 = m_ptrRots[1];
        Node* ptr2 = m_ptrRots[2];

        if (_getBalanceFactor(m_ptrRotHead)>0)
        {
            if (_getBalanceFactor(ptr1)<0)
                _rotate1(m_ptrRotHead, ptr1, SenseType::left);

            _rotate1(m_ptrParent, m_ptrRotHead, SenseType::right);
        }
        else
        {
            if (_getBalanceFactor(ptr1)>0)
                _rotate1(m_ptrRotHead, ptr1, SenseType::right);

            _rotate1(m_ptrParent, m_ptrRotHead, SenseType::left);
        }
    }
    m_ptrParent = nullptr;
    m_ptrRotHead = nullptr;
}

template<typename T>
void BinarySearchTree<T>::_rotate1(Node* parentNode, Node* node, SenseType sense)
{
    if (node != nullptr)
    {
        Node* node1 = nullptr;
        if (sense == SenseType::left)
        {
            if (node->right == nullptr)
                return;

            node1 = node->right;
            node->right = node1->left;
            node1->left = node;
            if (parentNode != nullptr)
            {
                if (parentNode->left == node)
                {
                    parentNode->left = node1;
                }
                else
                    parentNode->right = node1;
            }
            else
                m_root = node1;
        }
        else
        {
            if (node->left == nullptr)
                return;
            node1 = node->left;
            node->left = node1->right;
            node1->right = node;
            if (parentNode != nullptr)
            {
                if (parentNode->left == node)
                {
                    parentNode->left = node1;
                }
                else
                    parentNode->right = node1;
            }
            else
                m_root = node1;
        }
        _updateNodeHeight(node);
        _updateNodeHeight(node1);
        _updateNodeHeight(parentNode);
    }
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_findMinFrom(Node* node)
{
    while (node->left != nullptr)
        node = node->left;
    return node;
}

template<typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::_findMaxFrom(Node* node)
{
    while (node->right != nullptr)
        node = node->right;
    return node;
}

template<typename T>
void BinarySearchTree<T>::_updateNodeHeight(Node* node)
{
    if (node != nullptr)
    {
        int hL = _getNodeHeight(node->left);
        int hR = _getNodeHeight(node->right);
        node->m_height = std::max(hL,hR) + 1;
    }
}

template<typename T>
void BinarySearchTree<T>::_updateDepth(Node* node, int depth)
{
    if (node != nullptr)
    {
        node->m_depth = depth;
        _updateDepth(node->left, depth+1);
        _updateDepth(node->right, depth+1);
    }
}

template<typename T>
std::vector<T> BinarySearchTree<T>::getSequence()
{
    std::vector<T> ans{};
    if (m_root != nullptr)
    {
        std::queue<Node*> q{};
        q.emplace(m_root);

        while(!q.empty())
        {
            Node* node{ q.front() };

            if (node->left != nullptr)
                q.emplace(node->left);
            if (node->right != nullptr)
                q.emplace(node->right);

            ans.emplace_back(*(node->m_elementPointer->dataPointer));

            q.pop();
        }
    }
    return ans;
}

template<typename T>
void BinarySearchTree<T>::printTree()
{
    if (m_root != nullptr)
    {
        _updateDepth(m_root, 0);

        int nlines{ m_root->m_height + 1 };
        int pWidth{ 2 };
        for (int i{0}; i<nlines-1; i++)
            pWidth *= 2;
        pWidth -= 1;

        // Recreate canvas
        std::string line{""};
        for (int w{0}; w<pWidth; w++)
            for (int i{0}; i<printSpace; i++)
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
        std::cout << "Null tree\n";
}

template<typename T>
void BinarySearchTree<T>::_printNode(Node*& node, int x, int y)
{
    if (node != nullptr)
    {
        std::string& line{ m_lines[y] };
        std::size_t buf{ printSpace - node->m_label.size() };
        for (int i{0}; i<node->m_label.size(); i++)
            line[x*printSpace+buf+i] = node->m_label[i];

        int H{m_root->m_height};
        int d{node->m_depth};
        int shift{1};
        for (int i{0}; i<H-d-1; i++)
            shift *= 2;

        _printNode(node->left, x-shift, y+1);
        _printNode(node->right, x+shift, y+1);
    }
}