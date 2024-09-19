#include "Bst.h" // dummy
#include <algorithm>
#include <cmath>
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
void BinarySearchTree<T>::_insertNode(Node<T>*& node, const T& data, int depth, std::string label)
{
    if (node != nullptr)
    {
        if (data < node->m_data)
        {
            _insertNode(node->left, data, depth+1, label);
        }
        else
            _insertNode(node->right, data, depth+1, label);

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
        node = new Node{data, depth, label};
        if (label.size() > printSpace)
            printSpace = label.size();
    }
}

// Delete all nodes with data 'data'
template<typename T>
void BinarySearchTree<T>::deleteNode(const T& data)
{
    if (m_maxBalanceFactor>0)
    {
        std::size_t count{ findNodes(data) };
        for (std::size_t i{0}; i<count; i++)
        {
            m_root = _deleteNode(m_root, data);
            _rotate();
        }
    }
    else
        m_root = _deleteNodes(m_root, data);
}

template<typename T>
Node<T>* BinarySearchTree<T>::_deleteNode(Node<T>* node, const T& data)
{
    if (node != nullptr)
    {
        if (*node == data)
        {
            if (node->m_height == 0)
            {
                delete node;
                node = nullptr;
            }
            else if (node->left != nullptr && node->right == nullptr)
            {
                Node<T>* ptr{ node };
                node = node->left;
                delete ptr;
            }
            else if (node->left == nullptr && node->right != nullptr)
            {

                Node<T>* ptr{ node };
                node = node->right;
                delete ptr;
            }
            else
            {
                node->copyNode(node->left->findMax()); // no duplicates guaranteed in left subtree
                node->left = _deleteNode(node->left, node->m_data);
                // no change in depth
            }
        }
        else if (*node < data)
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
Node<T>* BinarySearchTree<T>::_deleteNodes(Node<T>* node, const T& data)
{
    if (node != nullptr)
    {
        if (*node == data)
        {
            if (node->m_height == 0)
            {
                delete node;
                node = nullptr;
            }
            else if (node->left != nullptr && node->right == nullptr)
            {
                Node<T>* ptr{ node };
                node = node->left;
                delete ptr;
            }
            else if (node->left == nullptr && node->right != nullptr)
            {
                Node<T>* ptr{ nullptr };
                while (node != nullptr && *node == data)
                {
                    ptr = node;
                    node = node->right;
                    delete ptr;
                }
            }
            else
            {
                node->copyNode(node->left->findMax()); // no duplicates guaranteed in left subtree
                node->left = _deleteNode(node->left, node->m_data);
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
    
    if (rotationLength > 2*m_maxBalanceFactor + 1)
        throw std::runtime_error("rotation length must be at most maximum balance factor + 2");

    m_rotationLength = rotationLength;
}

template<typename T>
void BinarySearchTree<T>::_deleteTree(Node<T>*& node)
{
    if (node != nullptr)
    {
        _deleteTree(node->left);
        _deleteTree(node->right);
        delete node;
        node = nullptr;
    }
}

template<typename T>
Node<T>* BinarySearchTree<T>::_getNode(const T& data)
{
    if (m_root != nullptr)
    {
        Node<T>* node{ m_root };

        while (true)
        {
            if (*node == data)
            {
                return node;
            }
            else if (*node < data)
            {
                if (node->right != nullptr)
                {
                    node = node->right;
                }
                else
                    return nullptr;
            }
            else
            {
                if (node->left != nullptr)
                {
                    node = node->left;
                }
                else
                    return nullptr;
            }
        }
    }
    else
        return nullptr;
}

template<typename T>
bool BinarySearchTree<T>::_isUnbalanced(Node<T>* node)
{
    if (node != nullptr)
    {
        int bf = _getBalanceFactor(node);
        return abs(bf) > m_maxBalanceFactor;
    }
    return false;
}

template<typename T>
std::size_t BinarySearchTree<T>::findNodes(const T& data)
{
    std::size_t ans{0};

    if (m_root != nullptr)
    {
        Node<T>* node{ m_root };

        while (true)
        {
            if (*node == data)
            {
                ans++;
                if (node->right != nullptr)
                {
                    node = node->right;    // duplicates may exist in right subtree
                }
                else
                    return ans;
            }
            else if (*node < data)
            {
                if (node->right != nullptr)
                {
                    node = node->right;
                }
                else
                    return ans;
            }
            else
            {
                if (node->left != nullptr)
                {
                    node = node->left;
                }
                else
                    return ans;
            }
        }
    }
    else
        return 0;
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
        Node<T>* ptr1{ nullptr };
        Node<T>* ptr2{ nullptr };

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

template<typename T>
void BinarySearchTree<T>::_rotateAVL()
{
    if (m_ptrRotHead != nullptr)
    {
        // std::cout << "ROTATING AT PIVOT " << m_ptrRotHead->m_label << '\n';
        _fillRotationPointers();
        Node<T>* ptr1 = m_ptrRots[1];
        Node<T>* ptr2 = m_ptrRots[2];

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
void BinarySearchTree<T>::_rotate1(Node<T>* parentNode, Node<T>* node, SenseType sense)
{
    if (node != nullptr)
    {
        Node<T>* node1 = nullptr;
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
void BinarySearchTree<T>::_setNodeHeight(Node<T>* node, int heightL, int heightR)
{
    if (node != nullptr)
        node->m_height = std::max(heightL,heightR) + 1;
}

template<typename T>
void BinarySearchTree<T>::_updateNodeHeight(Node<T>* node)
{
    if (node != nullptr)
    {
        int hL = _getNodeHeight(node->left);
        int hR = _getNodeHeight(node->right);
        node->m_height = std::max(hL,hR) + 1;
    }
}

// Update height values for all nodes with root 'node' inclusive
template<typename T>
void BinarySearchTree<T>::_updateHeight(Node<T>* node)
{
    if (node != nullptr)
    {
        _updateHeight(node->left);
        _updateHeight(node->right);
        _updateNodeHeight(node);
    }
}

template<typename T>
void BinarySearchTree<T>::_updateDepth(Node<T>* node, int depth)
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
        std::queue<Node<T>*> q{};
        q.emplace(m_root);

        while(!q.empty())
        {
            Node<T>* node{ q.front() };

            if (node->left != nullptr)
                q.emplace(node->left);
            if (node->right != nullptr)
                q.emplace(node->right);

            ans.emplace_back(node->getData());

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
void BinarySearchTree<T>::_printNode(Node<T>*& node, int x, int y)
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