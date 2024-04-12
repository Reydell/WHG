#pragma once

template <typename T>

struct Node
{
    T* value;
    Node<T>* next;

    Node (T* v, Node<T>* h):
        value(v),
        next(h)
    {}
};

template <typename T>

class List
{
public:
    Node<T>* head;
    Node<T>* currentIndex;

public:
    List ():
        head(nullptr),
        currentIndex(nullptr)
    {}

    // ~List()
    // {
        
    // }
    
    T* operator[] (int index)
    {
        Node<T>* p_res = head;

        for (int i = 1; i < index; i++)
        {
            p_res = p_res->next;
        }

        return p_res->value;
    }

    Node<T>* getNode(int index)    ///
    {
        if (!index)
            return head;
        
        Node<T>* p_res = head;

        for (int i = 1; i < index; i++)
        {
            p_res = p_res->next;
        }

        return p_res;
    }

    void pushFront(T* pValue)
    {
        head = new Node<T>(pValue, head);
    }

    T* popFront()
    {
        assert(head);

        Node<T>* newHead = head->next;
        T* value = head->value;
        delete head;
        head = newHead;

        return value;
    }

    // void removeByValue(T* value);

    // void removeByIndex(int i);

    int len()
    {
        if (!head)
            return 0;

        int res = 0;
        Node<T>* pCurrentNode = head;

        for (;;)
        {
            res++;

            pCurrentNode = pCurrentNode->next;
            if (!pCurrentNode)
                break;
        }

        return res;
    }

    void clear()
    {
        for (int i = this->len(); i > 0; i--)
        {
            delete this->getNode(i);
        }
        
        head = nullptr;
    }

/*  void insert(int index)
    {
        if (index)
    }*/

    // void erase();

    // void append(T* value);

    // void operator+= (T* value);

    // bool contains(T* value);
};