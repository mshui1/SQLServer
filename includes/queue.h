//Queue class implements its various functions using linked list structure of nodes

#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include "list.h"
using namespace std;

template <class T>
class Queue {
public:
    Queue() :_head(NULL), _tail(NULL) {}

    //destructor deletes entire queue
    ~Queue() {

        delete_all(_head);
    }

    //copy constructor copies over original queue
    Queue(const Queue<T>& other) {

        _head = nullptr;
        init_head(_head);
        _tail = nullptr;
        init_head(_tail);
        //assigns returned pointer (pointing to last node of queue) of copy_list fxn to queue's tail
        _tail = copy_list(other._head, _head);
    }

    //assignment operator copies over original queue
    Queue<T>& operator =(const Queue& rhs) {

        //assigns returned pointer (pointing to last node of queue) of copy_list fxn to queue's tail
        _tail = copy_list(rhs._head, _head);
        return *this;

    }

    void push(const T& item);

    T pop();

    bool empty();

    T front();

    friend ostream& operator <<(ostream& outs, const Queue& q) {
        return print_list(q._head, outs);
    }

private:
    node<T>* _head;
    node<T>* _tail;
};

//deletes item from front of queue and returns it, null is queue is empty
template<class T>
T Queue<T>::pop() {
    if (!empty()) {
        T temp = delete_head(_head);
        if (empty()) {

            _tail = NULL;
        }
        return temp;
    }
    else {
        //return NULL;
        return T();
    }
}

//returns true is queue is empty, false otherwise
template<class T>
bool  Queue<T>::empty() {
    if (_head == NULL) {
        return true;
    }
    else {
        return false;
    }
}

//returns the item at the front of queue, null if queue is empty
template<class T>
T Queue<T>::front() {
    if (!empty()) {
        return _head->_item;
    }
    else {
       /* return NULL;*/
        return T();
        
    }
}

//inserts item at the end of the queue
template<class T>
void Queue<T>::push(const T& item) {
    //assigns pointer (pointing to node of item that was inserted) from insert_after fxn to queue's tail
    _tail = insert_after(_head, _tail, item);


}

#endif