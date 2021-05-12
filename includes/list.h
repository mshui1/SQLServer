//various linked list functions constructed using node structure

#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <cassert>
using namespace std;

template <class T>
struct node {
    T _item;
    node<T>* _next;
    node(const T& item = T(), node<T>* next = NULL) :_item(item), _next(next) {}
    friend ostream& operator << (ostream& outs, const node<T>& print_me) {
        outs << "[" << print_me._item << "]->";
        return outs;
    }
};

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>*& head, T item) {
    //create new temp node
    node<T>* temp = new node<T>;

    //point temp's next to node where head is pointing
    temp->_next = head;

    //assign item to temp's item
    temp->_item = item;

    //point head to new node
    head = temp;

    return head;
}

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>*& head, node<T>* after, const T& item) {
    if (after == nullptr) {

        return insert_head(head, item);
    }
    else {

        //create new temp node
        node<T>* temp = new node<T>;

        //assign item to temp's item
        temp->_item = item;

        //point temp's next to after's next node
        temp->_next = after->_next;

        //point after to temp node
        after->_next = temp;

        //return pointer to newly created node
        return temp;
    }
}

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head) {

    if (head == NULL) {
        return true;
    }
    else return false;
}

//delete the node at the head of the list, return the item:
template <class T>
T delete_head(node<T>*& head)
{
    //make sure list is not empty
    assert(!empty(head));

    //store head node's item to temp variable
    T temp = head->_item;

    //point temporary pointer to head node
    node<T>* walker = head;

    //point head pointer to current head's next node
    head = head->_next;

    //delete original head node
    // delete walker;

    //return value in original head node
    return temp;
}

//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*& head) {
    while (!empty(head)) {
        //keep deleting head from list while list is not empty
        delete_head(head);
    }
}

//initializes head to NULL
template <typename T>
node<T>* init_head(node<T>*& head) {

    head = nullptr;
    return head;
}

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>*& cpy) {
    //point temporary walker pointer to head node
    const node<T>* walker = head;

    //point temp pointer to head of list that will be be copied into
    node<T>* temp = cpy;

    //while walker isn't null, insert new node with walker's item into list that will be copied into and point head of latter list to that new node, advance walker pointer 
    while (walker != nullptr) {
        cpy = insert_after(temp, cpy, walker->_item);
        walker = walker->_next;
    }

    //point temp2 pointer to last new node inserted
    node<T>* temp2 = cpy;

    //point cpy (that ended up pointing to last node inserted) back to head of list that will be copied into
    cpy = temp;

    //return pointer to last node inserted
    return temp2;
}

//print the list and return outs
template<class T>
ostream& print_list(const node<T>* head, ostream& outs = cout) {

    while (!empty(head)) {
        outs << *head;
        head = head->_next;
    }
    outs << "|||";
    return outs;
}

#endif