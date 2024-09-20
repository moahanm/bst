#include "Dictionary.h" // dummy


template<typename T, typename U>
void Dictionary<T,U>::emplace(const T& key, const U& value, std::string label)
{
    if (isEqualType<T,std::string>::value)  // workaround for partial template specification for member functions
        label = key;

    Node* node{ _getNode(key) };
    if (node != nullptr)
    {
        node->m_elementPointer->data->second = value;
        node->setLabel(label);
    }
    else
        this->insertNode(KeyValuePair<T,U>(key,value), label);
}


template<typename T, typename U>
void Dictionary<T,U>::erase(const T& key)
{
    Node* node{ _getNode(key) };
    if (node != nullptr)
        this->deleteNode(KeyValuePair<T,U>(key,node->m_elementPointer->data->second));
}


template<typename T, typename U>
typename BinarySearchTree<KeyValuePair<T,U>>::Node* Dictionary<T,U>::_getNode(const T& key)
{
    if (this->m_root != nullptr)
    {
        Node* node{ this->m_root };

        while (true)
        {
            if (*(node->m_elementPointer->data) == key)
            {
                return node;
            }
            else if (*(node->m_elementPointer->data) < key)
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