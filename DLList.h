#ifndef DLLIST_H
#define DLLIST_H

#include <iostream>


template<typename T>
class DLList
{
public:

    struct Element  // double linked list for element removal without search
    {
        T* dataPointer{ nullptr };
        Element* previous{ nullptr };
        Element* next{ nullptr };
    };

    class Iterator  // create iterator class to deference Element returning the data itself
    {
    private:
        Element* m_current;
    public:
        Iterator(Element* ptr): m_current{ptr}
        {
        }
        T& operator*(){ return *(m_current->dataPointer); }
        Iterator& operator++()
        {
            m_current = m_current->next;
            return *this;
        }
        Iterator& operator++(int)
        {
            m_current = m_current->next;
            return *this;
        }
        bool operator!=(const Iterator& it) const { return m_current != it.m_current; }
    };

    DLList()
    {
        head = new Element();
        tail  = head;
    }

    Iterator begin(){ return Iterator{head}; }
    
    Iterator end(){ return Iterator{tail}; }

    Element* push(const T& data)
    {
        Element* ele = new Element();   // new tail
        tail->dataPointer = new T{data};
        tail->next = ele;
        ele->previous = tail;
        tail = ele;
        return tail->previous;
    }

    void remove(Element* ptr)
    {
        if (ptr != tail)
        {
            if (ptr != head)
            {
                ptr->next->previous = ptr->previous;
                ptr->previous->next = ptr->next;
            }
            else
            {
                ptr->next->previous = nullptr;
                head = ptr->next;
            }

            delete ptr->dataPointer;
            delete ptr;
        }
    }

    virtual ~DLList()
    {
        Element* ptr{ nullptr };
        while (head->next != nullptr)
        {
            ptr = head;
            head = head->next;
            head->previous = nullptr;
            delete ptr->dataPointer;
            delete ptr;
        }
        delete head;  // which is also tail
        head = nullptr;
    }

private:
    Element* head;
    Element* tail;

};

#endif