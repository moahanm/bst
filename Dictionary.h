#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "Bst.h"
#include <exception>
#include <iostream>
#include "Meta.h"
#include <stdexcept>
#include <string>

/*
Dictionary is a hash table using BinarySearchTree for storing keys
*/

template<typename T, typename U>
class Dictionary;


// KeyValuePair must be declared external to Dictionary as BinarySearchTree needs this type
template<typename T, typename U>
class KeyValuePair
{
public:
    T first;
    U second;

private:
    KeyValuePair(const T& key, const U& value): first(key), second(value)
    {
    }

    KeyValuePair() = delete;

public:
    bool operator==(const KeyValuePair& kyp) const { return first == kyp.first; }

    bool operator==(const T& key) const { return first == key; }

    bool operator<(const KeyValuePair& kyp) const { return first < kyp.first; }

    bool operator<(const T& key) const { return first < key; }

public:
    virtual ~KeyValuePair()
    {
    }

    friend class Dictionary<T,U>;

};


template<typename T, typename U>
class Dictionary : public BinarySearchTree<KeyValuePair<T,U>>
{
using Node = typename BinarySearchTree<KeyValuePair<T,U>>::Node;

public:
    Dictionary(): BinarySearchTree<KeyValuePair<T,U>>()
    {
    }

    bool contains(const T& key){ return (_getNode(key) != nullptr) ? true : false; }

    void emplace(const T& key, const U& value, std::string label="");

    void erase(const T& key);

    U& operator[](const T& key)
    {
        Node* node{ _getNode(key) };
        if (node != nullptr)
        {
            return node->m_elementPointer->dataPointer->second;
        }
        else
            throw std::runtime_error("Key not found");
    }

    virtual ~Dictionary()
    {
    }

private:
    Node* _getNode(const T& key);

};


#include "Dictionary.cpp"


#endif