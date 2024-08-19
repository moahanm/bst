#include "Bst.h" // dummy
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


template<typename T>
void BinarySearchTree<T>::insertNode(T data)
{
    _insertNode(m_root, data, 0);
    std::cout << "INSERTED " << data << '\n';
    _rotate();
}

template<typename T>
void BinarySearchTree<T>::_insertNode(Node<T>*& node, T data, int depth)
{
    if (node != nullptr)
    {
        if (data < node->m_data)
        {
            _insertNode(node->smaller, data, depth+1);
        }
        else
            _insertNode(node->larger, data, depth+1);

        int lh = (node->smaller == nullptr) ? -1 : node->smaller->m_height;
        int rh = (node->larger == nullptr) ? -1 : node->larger->m_height;
        node->m_height = std::max(lh,rh) + 1;
        node->m_bf = lh - rh;

        if (m_ptrRot == nullptr)
        {
            if (abs(node->m_bf)>1)
            {
                m_ptrRot = node;
            }
        }
        else if (m_ptrParent == nullptr)
            m_ptrParent = node;
    }
    else
    {
        node = new Node{data, depth};
        m_ptrParent = nullptr;
        m_ptrRot = nullptr;
    }
}

// Delete all nodes with data 'data'
template<typename T>
void BinarySearchTree<T>::deleteNodes(T data)
{
    if (m_isBalanced)
    {
        std::size_t count{ findNodes(data) };
        for (std::size_t i{0}; i<count; i++)
        {
            m_root = _deleteNode(m_root, data);
            // _rotate();
        }
    }
    else
        m_root = _deleteNodes(m_root, data);
}

template<typename T>
Node<T>* BinarySearchTree<T>::_deleteNode(Node<T>* node, T data)
{
    if (node != nullptr)
    {
        if (node->isEqual(data))
        {
            if (node->m_height == 0)
            {
                delete node;
                node = nullptr;
            }
            else if (node->smaller != nullptr && node->larger == nullptr)
            {
                Node<T>* ptr{ node };
                node = node->smaller;
                delete ptr;
                if (node != nullptr)
                    _updateDepth(node, node->m_depth-1);
            }
            else if (node->smaller == nullptr && node->larger != nullptr)
            {

                Node<T>* ptr{ node };
                node = node->larger;
                delete ptr;
                if (node != nullptr)
                    _updateDepth(node, node->m_depth-1);
            }
            else
            {
                node->setData(node->smaller->findMax()); // no duplicates guaranteed in smaller subtree
                node->smaller = _deleteNode(node->smaller, node->m_data);
                // no change in depth
            }
        }
        else if (node->isLessThan(data))
        {
            node->larger = _deleteNode(node->larger, data);
        }
        else
        {
            node->smaller = _deleteNode(node->smaller, data);
        }

        if (node != nullptr)
        {
            int lh = (node->smaller == nullptr) ? -1 : node->smaller->m_height;
            int rh = (node->larger == nullptr) ? -1 : node->larger->m_height;
            node->m_height = std::max(lh,rh) + 1;
            node->m_bf = lh - rh;
        }

        return node;
    }
    else
        return nullptr;
}

template<typename T>
Node<T>* BinarySearchTree<T>::_deleteNodes(Node<T>* node, T data)
{
    if (node != nullptr)
    {
        if (node->isEqual(data))
        {
            if (node->m_height == 0)
            {
                delete node;
                node = nullptr;
            }
            else if (node->smaller != nullptr && node->larger == nullptr)
            {
                Node<T>* ptr{ node };
                node = node->smaller;
                delete ptr;
                if (node != nullptr)
                    _updateDepth(node, node->m_depth-1);
            }
            else if (node->smaller == nullptr && node->larger != nullptr)
            {
                Node<T>* ptr{ nullptr };
                int d{0};
                while (node != nullptr && node->isEqual(data))
                {
                    ptr = node;
                    node = node->larger;
                    delete ptr;
                    d++;
                }
                if (node != nullptr)
                    _updateDepth(node, node->m_depth-d);
            }
            else
            {
                node->setData(node->smaller->findMax()); // no duplicates guaranteed in smaller subtree
                node->smaller = _deleteNode(node->smaller, node->m_data);
                // no change in depth
            }
        }

        if (node != nullptr)
        {
            node->smaller = _deleteNodes(node->smaller, data);
            node->larger = _deleteNodes(node->larger, data);

            int lh = (node->smaller == nullptr) ? -1 : node->smaller->m_height;
            int rh = (node->larger == nullptr) ? -1 : node->larger->m_height;
            node->m_height = std::max(lh,rh) + 1;
            node->m_bf = lh - rh;
        }

        return node;
    }
    else
        return nullptr;
}

template<typename T>
void BinarySearchTree<T>::_deleteTree(Node<T>*& node)
{
    if (node != nullptr)
    {
        _deleteTree(node->smaller);
        _deleteTree(node->larger);
        delete node;
        node = nullptr;
    }
}

template<typename T>
std::size_t BinarySearchTree<T>::findNodes(T data)
{
    std::size_t ans{0};

    if (m_root != nullptr)
    {
        Node<T>* node{ m_root };

        while (true)
        {
            if (node->isEqual(data))
            {
                ans++;
                if (node->larger != nullptr)
                {
                    node = node->larger;    // duplicates may exist in larger subtree
                }
                else
                    return ans;
            }
            else if (node->isLessThan(data))
            {
                if (node->larger != nullptr)
                {
                    node = node->larger;
                }
                else
                    return ans;
            }
            else
            {
                if (node->smaller != nullptr)
                {
                    node = node->smaller;
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
void BinarySearchTree<T>::_rotate()
{
    if (m_ptrRot != nullptr)
    {
        Node<T>* ptr1 = (m_ptrRot->m_bf>0) ? m_ptrRot->smaller : m_ptrRot->larger;
        Node<T>* ptr2 = (ptr1->m_bf>0) ? ptr1->smaller : ptr1->larger;

        if (m_ptrRot->m_bf>0 && ptr1->m_bf>0)   // R
        {
            m_ptrRot->smaller = ptr1->larger;
            ptr1->larger = m_ptrRot;
            if (m_ptrParent != nullptr)
            {
                if (m_ptrParent->smaller == m_ptrRot)
                {
                    m_ptrParent->smaller = ptr1;
                }
                else
                    m_ptrParent->larger = ptr1;
            }
            else
                m_root = ptr1;

            _updateHeight(ptr1);
            _updateDepth(ptr1, ptr1->m_depth-1);
        }
        else if (m_ptrRot->m_bf>0 && ptr1->m_bf<0)  // LR
        {
            m_ptrRot->smaller = ptr2->larger;
            ptr1->larger = ptr2->smaller;
            ptr2->smaller = ptr1;
            ptr2->larger = m_ptrRot;
            if (m_ptrParent != nullptr)
            {
                if (m_ptrParent->smaller == m_ptrRot)
                {
                    m_ptrParent->smaller = ptr2;
                }
                else
                    m_ptrParent->larger = ptr2;
            }
            else
                m_root = ptr2;

            _updateHeight(ptr2);
            _updateDepth(ptr2, ptr2->m_depth-2);
        }
        else if (m_ptrRot->m_bf<0 && ptr1->m_bf>0)  // RL
        {
            m_ptrRot->larger = ptr2->smaller;
            ptr1->smaller = ptr2->larger;
            ptr2->smaller = m_ptrRot;
            ptr2->larger = ptr1;
            if (m_ptrParent != nullptr)
            {
                if (m_ptrParent->smaller == m_ptrRot)
                {
                    m_ptrParent->smaller = ptr2;
                }
                else
                    m_ptrParent->larger = ptr2;
            }
            else
                m_root = ptr2;

            _updateHeight(ptr2);
            _updateDepth(ptr2, ptr2->m_depth-2);
        }
        else    // L
        {
            m_ptrRot->larger = ptr1->smaller;
            ptr1->smaller = m_ptrRot;
            if (m_ptrParent != nullptr)
            {
                if (m_ptrParent->smaller == m_ptrRot)
                {
                    m_ptrParent->smaller = ptr1;
                }
                else
                    m_ptrParent->larger = ptr1;
            }
            else
                m_root = ptr1;

            _updateHeight(ptr1);
            _updateDepth(ptr1, ptr1->m_depth-1);
        }
    }
}

template<typename T>
void BinarySearchTree<T>::updateHeight()
{
    _updateHeight(m_root);
}

// Update height values for all nodes with root 'node' inclusive
template<typename T>
void BinarySearchTree<T>::_updateHeight(Node<T>* node)
{
    if (node != nullptr)
    {
        _updateHeight(node->smaller);
        _updateHeight(node->larger);

        if (node->smaller == nullptr && node->larger == nullptr)    // leaf
        {
            node->m_height = 0;
            node->m_bf = 0;
        }
        else if (node->smaller == nullptr && node->larger != nullptr)
        {
            node->m_height = node->larger->m_height + 1;
            node->m_bf = -1 - node->larger->m_height;
        }
        else if (node->smaller != nullptr && node->larger == nullptr)
        {
            node->m_height = node->smaller->m_height + 1;
            node->m_bf = node->smaller->m_height + 1;
        }
        else
        {
            node->m_height = std::max(node->smaller->m_height,node->larger->m_height) + 1;
            node->m_bf = node->smaller->m_height - node->larger->m_height;
        }
    }
}

template<typename T>
void BinarySearchTree<T>::_updateDepth(Node<T>* node, int depth)
{
    if (node != nullptr)
    {
        node->m_depth = depth;
        _updateDepth(node->smaller, depth+1);
        _updateDepth(node->larger, depth+1);
    }
}

template<typename T>
void BinarySearchTree<T>::printTree()
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

template<typename T>
void BinarySearchTree<T>::_printNode(Node<T>*& node, int x, int y)
{
    if (node != nullptr)
    {
        std::string& line{ m_lines[y] };
        for (int i{0}; i<PRINTSPACE; i++)
            line[x*PRINTSPACE+i] = node->m_label[i];

        int H{m_root->m_height};
        int d{node->m_depth};
        int shift{1};
        for (int i{0}; i<H-d-1; i++)
            shift *= 2;

        _printNode(node->smaller, x-shift, y+1);
        _printNode(node->larger, x+shift, y+1);
    }
}