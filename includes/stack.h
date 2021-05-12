//Stack class implements its various functions using linked list structure of nodes

#ifndef STACK_H
#define STACK_H
#include <iostream>
#include "list.h"
using namespace std;

template <class T>
class Stack
{
public:
    Stack() :_top(NULL) {}

    ~Stack();

    //copy constructor copies over original stack
    Stack(const Stack<T>& other) {
        _top;
        init_head(_top);
        copy_list(other._top, _top);
    }

    //assignment operator copies over original stack
    Stack<T>& operator =(const Stack<T>& rhs) {

        copy_list(rhs._top, _top);
        return *this;
    }

    void push(const T& item);

    bool empty();

    friend ostream& operator << (ostream& outs, const Stack& s) {
        return print_list(s._top, outs);
    }

    T pop();
    T top();

private:
    node<T>* _top;
};

//destructor deletes the entire stack
template<class T>
Stack<T>::~Stack() {
    delete_all(_top);
}

//inserts item onto top of stack
template<class T>
void Stack<T>::push(const T& item) {
    insert_head(_top, item);
}

//deletes the top item from stack and returns it, null if stack is empty
template<class T>
T Stack<T>::pop() {
    if (!empty()) {
        return delete_head(_top);
    }
    else {
        return T(NULL);
    }
}

//returns the top item in stack and null if stack is empty
template<class T>
T Stack<T>::top() {
    if (!empty()) {
        return _top->_item;
    }
    else {
        return NULL;
    }
}

//returns true if stack is empty and false if not empty
template<class T>
bool Stack<T>::empty() {
    if (_top == nullptr) {
        return true;
    }
    else {
        return false;
    }
}

#endif // STACK_H