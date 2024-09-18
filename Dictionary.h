#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "Bst.h"
#include <exception>
#include <iostream>
#include <stdexcept>

/*
UNDER CONSTRUCTION
*/


template<typename T, typename U>
class Dictionary;

template<typename T, typename U>
class KeyValuePair
{
    typedef bool (*LessThanFunction)(T a, T b);

private:
    T m_key{};
    U m_value{};
    LessThanFunction m_LessThanFunction{ nullptr };

    KeyValuePair() = delete;

    KeyValuePair(T key, U value): m_key{key}, m_value{value}
    {
    }

    void setLessThanFunction(void* lessThanFunction)
    {
        m_LessThanFunction = reinterpret_cast<LessThanFunction>(lessThanFunction);
    }

    bool operator<(const KeyValuePair& kvp)
    {
        if (m_LessThanFunction == nullptr)
        {
            return m_key < kvp.m_key;
        }
        else
            return m_LessThanFunction(m_key,kvp.m_key);
    }
    std::ostream& operator<<(std::ostream& os)
    {
        return os << "(" << m_key << "," << m_value << ")";
    }

    virtual ~KeyValuePair()
    {
    }

    friend class Dictionary<T,U>;
};

template<typename T, typename U>
class Dictionary : public BinarySearchTree<KeyValuePair<T,U>>
{
public:
    Dictionary(): BinarySearchTree<KeyValuePair<T,U>>()
    {
    }

    U& operator[](const T key)
    {
        Node<U>* node{ _getNode(key) };
        if (node != nullptr)
        {
            return node->getData().m_value;
        }
        else
            throw std::runtime_error("Key not found");
    }

    virtual ~Dictionary()
    {
    }

};


#endif