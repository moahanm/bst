#ifndef DLLIST_H
#define DLLIST_H

#include <iostream>


template<typename T>
class DLList
{
public:

    struct Element  // double linked list for element removal without search
    {
        T* data{ nullptr };
        Element* previous{ nullptr };
        Element* next{ nullptr };
    };

    class Iterator  // create iterator class to deference Element returning the data itself
    {
    private:
        Element* current;
    public:
        Iterator(Element* ptr): current{ptr}
        {
        }
        T& operator*(){ return *(current->data); }
        Iterator& operator++()
        {
            current = current->next;
            return *this;
        }
        Iterator& operator++(int)
        {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator& it) const { return current != it.current; }
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
        tail->data = new T{data};
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

            delete ptr->data;
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
            delete ptr->data;
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