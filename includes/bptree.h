#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include "array_functions.h"
#include <iomanip>
#include <cassert>
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator {
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it = NULL, int _key_ptr = 0) :it(_it), key_ptr(_key_ptr) {}

        //SHOULD RETURN PAIR AS TYPE OF T?
        T operator *() {
            if (is_null()) {
              /*   return NULL;*/
                return T();
            }
            else {
                assert(key_ptr < it->data_count);
                return it->data[key_ptr];
            }
        }

        //WHAT IS un_used FOR?
        Iterator operator++(int un_used) {
            Iterator temp = *this;
            key_ptr++;
            //CONDITIONAL CORRECT?
            if (key_ptr > it->data_count - 1) {

                key_ptr = 0;
                it = it->next;
            }
            //SHOULD RESTART FROM BEGINNING AFTER REACHING END?

            return temp;
        }

        //ORDER HERE CORRECT?
        Iterator operator++() {
            key_ptr++;
            //CONDITIONAL CORRECT?
            if (key_ptr > it->data_count - 1) {

                key_ptr = 0;
                it = it->next;
            }
            //SHOULD RESTART FROM BEGINNING AFTER REACHING END?

            return *this;

        }


        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            //IS THIS RIGHT? / OR SHOULD IT TEST FOR EQUALITY BETWEEN it->data[key_ptr]s
            if (lhs.it == rhs.it) {
                return lhs.key_ptr == rhs.key_ptr;
            }
            else {
                return false;
            }

        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            //IS THIS RIGHT? / OR SHOULD IT TEST FOR EQUALITY BETWEEN it->data[key_ptr]s
            if (lhs.it != rhs.it) {

                return true;
            }
            else {
                return lhs.key_ptr != rhs.key_ptr;
            }
        }
        void print_Iterator() {
            //SHOULD CHECK IF it IS NULL BEFORE PRINTING?
            cout << it->data[key_ptr];
        }
        bool is_null() { return !it; }
    private:
        BPlusTree<T>* it;
        int key_ptr;
    };
public:
   
    BPlusTree(bool dups=true) :dups_ok(dups), data_count(0),
        child_count(0), next(NULL) {
        //left emtpy
        key_count = 0;
        child_count = 0;
    }
    //big three:
    BPlusTree(const BPlusTree<T>& other) {

    /*    cout << "COPY CTOR FIRED" << endl;*/
        copy_tree(other);
   }
    ~BPlusTree() {

        clear_tree();
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS) {
       /* cout << "ASSIGNMENT OPERATOR FIRED" << endl;*/
        //WHY DOES ASSIGNMENT OPERATOR WORK HERE WITHOUT RETURNING BTREE ITEM?
        // BTree<T> temp(RHS);
        // return temp;
        copy_tree(RHS);
        return *this;

    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node);
    bool contains(const T& entry) const;        //true if entry can be found in the array

    T& get(const T& entry);                     //return a reference to entry in the tree
    const T& get(const T& entry)const;          //return a reference to entry in the tree
   /* T& get_existing(const T& entry);  */          //return a reference to entry in the tree

    Iterator find(const T& entry);              //return an iterator to this key. NULL if not there.


    Iterator lower_bound(const T& key, bool check_biggest=true);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist >= entry
    Iterator upper_bound(const T& key, bool check_biggest=true);  //return first that goes AFTER key
    int size() const {
        return key_count;
    }//count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    void print_tree(int level = 0,
        ostream& outs = cout) const; //print a readable version of the tree
    friend ostream& operator<<(ostream& outs,
        const BPlusTree<T>& print_me) {
        print_me.print_tree(0, outs);
        return outs;
    }
    bool is_valid();
    bool root_valid();

    Iterator begin();
    Iterator end();
    T* find_ptr(const T& entry);
    const T* find_ptr(const T& entry) const;

    T& bigger_child();



private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    int key_count;
    bool is_leaf() const
    {
        return child_count == 0;
    }        //true if this is a leaf node

    /*T* find_ptr(const T& entry);  */      //return a pointer to this key. NULL if not there.

    //insert element functions
    bool loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage of data elements in child i
                                        // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    BPlusTree<T>* get_biggest_node();
    void get_smallest(T& entry);        //entry := leftmost leaf
    void get_biggest(T& entry);         //entry := rightmost leaf
    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void transfer_left(int i);          //transfer one element LEFT from child i
    void transfer_right(int i);         //transfer one element RIGHT from child i
    BPlusTree<T>* merge_with_next_subset(int i);    //merge subset i with subset i+1

    T& smallest_child();
    T smallest_child_by_value();
  /*  T& bigger_child();*/

};



template<typename T>
T* BPlusTree<T>::find_ptr(const T& entry) {

    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);
    if (found) {

        if (is_leaf()) {
            return &data[i];
        }
        else {
            return subset[i + 1]->find_ptr(entry);
        }
    }
    else {

        if (is_leaf()) {
            return NULL;
        }
        else {
            return subset[i]->find_ptr(entry);
        }
    }
}

template<typename T>
const T* BPlusTree<T>::find_ptr(const T& entry)const {

    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);
    if (found) {

        if (is_leaf()) {
            return &data[i];
        }
        else {
            return subset[i + 1]->find_ptr(entry);
        }
    }
    else {

        if (is_leaf()) {
            return NULL;
        }
        else {
            return subset[i]->find_ptr(entry);
        }
    }
}

template <typename T>
void BPlusTree<T>::insert(const T& entry) {
    //in order for this class to be able to keep track of the number of the keys,
    //      this function (and the functions it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values)
    //  then the success code must distinguish between inserting a new key, or
    //  adding a new key to the existing key. (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns,
    //  all the subtrees are valid
    //  btree subtrees EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])

   
    if (loose_insert(entry)) {
        key_count++;
    }

    if (data_count > MAXIMUM) {
        BPlusTree<T>* temp = new BPlusTree<T>;
        copy_array(temp->data, data, temp->data_count, data_count);
        //CAN USE COPY ARRAY FUNCTION TO COPY ARRAY OF BTREE POINTERS?/WILL CAUSE PROBLEMS LATER?
        copy_array(temp->subset, subset, temp->child_count, child_count);
        data_count = 0;
        //NEED TO ALSO DELETE POINTERS LEFT IN ROOT'S SUBSET? / WILL CAUSE PROBLEMS LATER IF DELETED? / IF NOT DELETED? OR
        //NEED TO ALSO RESET POINTERS LEFT IN ROOT'S SUBSET TO NULL? / WILL CAUSE PROBLEMS LATER IF RESET? / IF NOT RESET?
        child_count = 1;
        subset[0] = temp;

        fix_excess(0);




    }



    
}

template <typename T>
bool BPlusTree<T>::loose_insert(const T& entry) {
    /*
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         found
         a. found/leaf: deal with duplicates: call +
         b. found/no leaf: subset[i+1]->loose_insert(entry)
                           fix_excess(i+1) if there is a need

         ! found:
         c. !found / leaf : insert entry in data at position i
         c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert(i+1)|  loose_insert(i) |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */


    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);

    if (found) {

        if (is_leaf()) {
            if (dups_ok) {
                data[i] = data[i] + entry;
          /*      data[i] = entry;*/
            }
            return false;
        }
        else {
            bool temp;
            temp=subset[i + 1]->loose_insert(entry);
            fix_excess(i + 1);
            return temp;
        }
    }
    else {
        if (is_leaf()) {
            insert_item(data, i, data_count, entry);
            return true;
        }
        else {
            bool temp;
            temp=subset[i]->loose_insert(entry);
            fix_excess(i);
            return temp;
        }
    }

    const bool debug = false;
}

template <typename T>
void BPlusTree<T>::fix_excess(int i) {
    //this node's child i has one too many items: 3 steps:
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    //      subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert
    //      it into this node's data[]
    //
    //Note that this last step may cause this node to have too many itesm.
    //  This is OK. This will be
    //  dealt with at the higher recursive level. (my parent will fix it!)
    //
    //NOTE: ODD/EVENNESS
    // when there is an excess, data_count will always be odd
    //  and child_count will always be even.
    //  so, splitting is always the same.

    //  000 B_PLUS_TREE
    //  if (subset[i]->is_leaf())
    //  transfer the middle entry to the right and...
    //  Deal with next pointers. just like linked list insert

    if (subset[i]->data_count > MAXIMUM) {

        insert_item(subset, i + 1, child_count, new BPlusTree<T>);
        split(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
        split(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
        T temp;
        detach_item(subset[i]->data, subset[i]->data_count, temp); //USE DELETE_ITEM ARRAY FXN HERE INSTEAD?
        ordered_insert(data, data_count, temp); //USE ORDERED INSERT ARRAY FXN HERE?
        if (subset[i]->is_leaf()) {
            insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
            subset[i + 1]->next = subset[i]->next;
            subset[i]->next = subset[i + 1];
        }

    }

}

template <typename T>
bool BPlusTree<T>::contains(const T& entry)const {
    if (find_ptr(entry) != NULL) {
        return true;

    }
    else {
        return false;
    }
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other) {
    //assert(empty());
    BPlusTree<T>* temp= nullptr;
    copy_tree(other, temp);

}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node) {
    next = other.next;
    key_count = other.key_count;
    child_count = other.child_count;
    copy_array(data, other.data, data_count, other.data_count);
    if (!other.is_leaf()) {

        child_count = other.child_count;
        for (int i = 0; i < child_count; i++) {

            subset[i] = new BPlusTree<T>();
            //WHY UNDEREFERENCED POINTER ALSO WORKS HERE?
            subset[i]->copy_tree(*other.subset[i], last_node);

        }
    }
    else {
        if (last_node == nullptr) {
            //TYPE MISMATCH HERE? (last_node IS A POINTER REF WHILE this IS A POINTER)
            last_node = this;
        }
        else {
            last_node->next = this;
            last_node = this;
        }
    }

}

template <typename T>
T& BPlusTree<T>::get(const T& entry) {
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));

    const bool debug = false;
    if (!contains(entry))
        insert(entry);

    return *(find_ptr(entry));
}

template <typename T>
const T& BPlusTree<T>::get(const T& entry)const {
    if (!contains(entry)) {
        //IS THIS CONDITIONAL NECESSARY? /NECESSARY TO RETURN A T ITEM HERE IF BELOW CAN TAKE CARE?
        return T();
    }
    else {
        //CAN THIS RETURN A T ITEM IF entry NOT FOUND?
        return *(find_ptr(entry));
    }


}

template<class T>
void BPlusTree<T>::print_tree(int level, ostream& outs) const {

    if (!is_leaf()) {
        subset[child_count - 1]->print_tree(level + 1);

        for (int i = data_count - 1; i >= 0; i--) {

            // outs << setw(50 * level)<<" this: "<<this<<" "<<subset[0]<<" and "<<subset[1]<<" "<<data[i]<<" cc: "<<child_count<<" dc: "<<data_count<<endl;
            outs << setw(15 * level) <<"["<< data[i]<<"]" <<endl;

            subset[i]->print_tree(level + 1);


        }

    }
    else {
   /*     outs << setw(16 * level) << "dc: " << data_count << endl;*/
        for (int i = data_count - 1; i >= 0; i--) {
            outs << setw(16 * level) << "["<<data[i]<<"]" << endl;

        }
        // outs << setw(10 * level)<<data[0]<<endl;
        // print_array(data,data_count);

    }

}

template <typename T>
bool BPlusTree<T>::empty() const {
    return (data_count == 0);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin() {
    return Iterator(get_smallest_node());
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end() {
    Iterator temp(NULL);
    return temp;
}


template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound( const T& key, bool check_biggest) {
    if (check_biggest) {
        BPlusTree<T>* biggest_node = get_biggest_node();
        int biggest_key_index = biggest_node->data_count - 1;
        if (key>biggest_node->data[biggest_key_index]) {
            return Iterator(biggest_node, biggest_key_index);
        }

    }
    int i = first_ge(data, data_count, key);
    bool found = (i < data_count && data[i] == key);
    if (found) {

        if (is_leaf()) {
            return Iterator(this, i);

        }
        else {
     /*       int child_i = first_ge(subset[i + 1]->data, subset[i + 1]->data_count, key);
                bool child_found = (child_i < subset[i + 1]->data_count && subset[i + 1]->data[child_i] == key);
                if (!child_found) {
                    if (subset[i + 1]->is_leaf()) {
                        if (child_i >= subset[i + 1]->data_count) {
                            
                            return subset[i + 2]->lower_bound(key, false);
                        }

                    }
            }*/


            return subset[i + 1]->lower_bound(key, false);
        }
    }
    else {

        if (is_leaf()) {
            if (i >= data_count) {
                Iterator it(this, data_count - 1);
                it++;
                return it;


            }
            else {
                return Iterator(this, i);
            }

        }
        else {

            //int child_i = first_ge(subset[i]->data, subset[i]->data_count, key);
            //bool child_found = (child_i < subset[i]->data_count && subset[i]->data[child_i] == key);
            //    if (!child_found) {
            //        if (subset[i]->is_leaf()) {
            //            if (child_i >= subset[i]->data_count) {
            //                return subset[i + 1]->lower_bound(key, false);
            //            }

            //        }
            //    }
            return subset[i]->lower_bound(key, false);
        }
    }



}

//return first that goes AFTER key
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key, bool check_biggest) {
    if (check_biggest) {
        BPlusTree<T>* biggest_node = get_biggest_node();
        int biggest_key_index = biggest_node->data_count - 1;
        if (key > biggest_node->data[biggest_key_index]) {
            return Iterator(biggest_node, biggest_key_index);
        }

    }

    int i = first_ge(data, data_count, key);
    bool found = (i < data_count && data[i] == key);
    if (found) {

        if (is_leaf()) {
            Iterator it(this, i);
            it++;
            if (it == end()) {
                return Iterator(this, i);
            }
            else {
                return it;
            }

        }
        else {
            return subset[i + 1]->upper_bound(key, false);
        }
    }
    else {

        if (is_leaf()) {
            if (i >= data_count) {
                Iterator it(this, data_count - 1);
                it++;
                if (it == end()) {
                    cout << "stop here" << endl;
                }
                return it;


            }
            else {
                Iterator it (this, i);
                if (it == end()) {

                    cout << "stop there" << endl;
                }
                return it;
            }
        }
        else {
            return subset[i]->upper_bound(key, false);
        }
    }

}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry) {
    //SHOULD INCLUDE key_ptr POSITION OF entry IN data ARRAY BELOW? / HOW TO FIND IF SO?
       int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);
    if (found) {

        if (is_leaf()) {
            return Iterator(this, i);
        
        }
        else {
            return subset[i + 1]->find(entry);
        }
    }
    else {

        if (is_leaf()) {
            Iterator temp(NULL);
            return temp;
        }
        else {
            return subset[i]->find(entry);
        }
    }
}


template <typename T>
BPlusTree<T>* BPlusTree<T>::get_biggest_node() {

    if (is_leaf())
    {
        return this;
    }
    else {
        return subset[child_count-1]->get_biggest_node();
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node() {

    if (is_leaf())
    {   
        if (empty()) {
            return NULL;
        }
        return this;
    }
    else {
        return subset[0]->get_smallest_node();
    }
}

template <typename T>
bool BPlusTree<T>::root_valid() {
    if (data_count == 0 && is_leaf()) { return true; }

    if (data_count < 1 || data_count > MAXIMUM) {

        return false;
    }
    if (is_leaf()) {
        return true;
    }
    else {

        Iterator it = begin();
        while (it != end()) {
            T temp = *it;
            it++;
            if (it != end()) {
                if (temp >= *it) {
                    return false;
                }
            }
            
        }
    
        
        

        if (child_count != data_count + 1) {
            return false;
        }
        if (!is_le(subset[child_count - 1]->data, subset[child_count - 1]->data_count, data[data_count - 1])) 
        {
            return false;
        }


        for (int i = 0; i < data_count; i++) {
            if (!is_gt(subset[i]->data, subset[i]->data_count, data[i])) {
                return false;
            }
    

            if (subset[i + 1]->smallest_child() != data[i]) {
                return false;
            }
            if (subset[i]->bigger_child() > data[i]) {
                return false;
            }
        }
        bool temp;
        for (int i = 0; i < child_count; i++) {
            temp = subset[i]->is_valid();
            if (temp == false) {
                break;
            }
        }

        return temp;

    }


}

template <typename T>
bool BPlusTree<T>::is_valid() {
    if (data_count<MINIMUM || data_count>MAXIMUM) {
        return false;
    }
    if (!is_sorted(data, data_count)) {
        return false;
    }

    if (!is_leaf()) {
        if (child_count != data_count + 1) {
            return false;
        }

        if (!is_le(subset[child_count - 1]->data, subset[child_count - 1]->data_count, data[data_count - 1])) {
            return false;
        }

        for (int i = 0; i < data_count; i++) {
            if (!is_gt(subset[i]->data, subset[i]->data_count, data[i])) {
                return false;
            }
          

            if (subset[i + 1]->smallest_child() != data[i]) {
                return false;
            }
            if (subset[i]->bigger_child() > data[i]) {
                return false;
            }
        }
        bool temp;
        for (int i = 0; i < child_count; i++) {
            temp = subset[i]->is_valid();
            if (temp == false) {
                break;
            }
        }
        /*    if (temp == false) {
                cout << "TEMP RETURNED FALSE" << endl;
            }*/
        return temp;
    }
    else {
        return true;
    }

}

template <typename T>
T& BPlusTree<T>::smallest_child() {
    if (is_leaf()) {
        return data[0];
    }

    else {
        return subset[0]->smallest_child();
    }

}

template <typename T>
T BPlusTree<T>::smallest_child_by_value() {
    if (is_leaf()) {
        assert(!empty());
        return data[0];
    }

    else {
        return subset[0]->smallest_child_by_value();
    }

}

template<typename T>
T& BPlusTree<T>::bigger_child() {
    if (is_leaf()) {
        return data[data_count - 1];
    }
    else {
        return subset[child_count - 1]->bigger_child();
    }
}

template <typename T>
void BPlusTree<T>::clear_tree() {
    key_count = 0;
    if (is_leaf()) {

        data_count = 0;
    }
    else {
        for (int i = 0; i < child_count; i++) {
            subset[i]->clear_tree();
            delete subset[i];
        }
        child_count = 0;
        data_count = 0;

    }

}


template <typename T>
void BPlusTree<T>::remove(const T& entry) {
    /*
     * ---------------------------------------------------------------------------------
     * Same as BTree:
     * Loose_remove the entry from this tree.
     * Shrink if you have to
     * ---------------------------------------------------------------------------------
     * once you return from loose_remove, the root (this object) may have no data and
     * only a single subset:
     * now, the tree must shrink:
     *
     * point a temporary pointer (shrink_ptr) and point it to this root's only subset
     * copy all the data and subsets of this subset into the root (through shrink_ptr)
     * now, the root contains all the data and poiners of it's old child.
     * now, simply delete shrink_ptr, and the tree has shrunk by one level.
     * Note, the root node of the tree will always be the same, it's the
     * child node we delete
     *
     *
     */


    if (contains(entry)) {
        key_count--;
    }
    

    loose_remove(entry);
    if ((data_count == 0) && (child_count == 1)) {

        BPlusTree<T>* shrink_ptr = subset[0];
        for (int i = 0; i < shrink_ptr->data_count; i++) {
            data[i] = shrink_ptr->data[i];

        }
        //WILL NOT OVERRIDE SUBSET ARRAY ELEMENT POINTING TO CHILD NODE?
        for (int i = 0; i < shrink_ptr->child_count; i++) {
            subset[i] = shrink_ptr->subset[i];
        }
        data_count = shrink_ptr->data_count;
        child_count = shrink_ptr->child_count;
        shrink_ptr->data_count = 0;
        //REALLY NEED TO SET SHRINK PTR'S CHILD COUNT TO 0?
        shrink_ptr->child_count = 0;
        delete shrink_ptr;


    }



}

template <typename T>
void BPlusTree<T>::loose_remove(const T& entry) {
    /* four cases:
          leaves:
               a. not found: there is nothing to do
               b. found    : just remove the target
          non leaf:
               c. not found: subset[i]->loose_remove, fix_shortage(i)
               d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                   (No More remove_biggest)

            |   !found               |   found                 |
      ------|------------------------|-------------------------|-------
      leaf  |  a: nothing            | b: delete               |
            |     to do              |    target               |
      ------|------------------------|-------------------------|-------
      !leaf | c:                     | d: B_PLUS_TREE          |
            |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
            |  fix_shortage(i)       | fix_shortage(i+1) [...] |
      ------|------------------------|-------------------------|-------


    */
    const bool debug = false;
    T found_entry;
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);

    if (is_leaf()) {
        if (!found) {
            //[a.] nothing to do
            //entry not in this tree
        }
        else {
            //[b.] just delete the item
            //   my parent will take care of my shortage

            T throwaway;
            delete_item(data, i, data_count, throwaway);
            key_count--;
      /*      assert(data_count <= MAXIMUM);
            return true;*/
        }

    }
    else {
        //not leaf:
        // ---- 000 B_PLUS_TREE: no more remove_biggest
        if (!found) {
            //[c.] Not here: subset[i]->loose_remove
            //not found yet. search in the next level:

            subset[i]->loose_remove(entry);
            fix_shortage(i);
        }
        else {
            //[d.] found key in an inner node:subset[i+1]->loose_remove
            assert(i < child_count - 1);

            /*
             * Here's The Story:
             * data[i] is the same as the item that we have deleted.
             * so, now, it needs to be replaced by the current smallest key
             *      in subset[i+1]
             * Problem: After fix_shortage, data[i] could have moved to a different index(?)
             *      or it could have sunk down to a lower level as a result of merge
             *      we need to find this item and replace it.
             *
             *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
             *      before fix_excess because this smallest key could be the very entry
             *      we are removing. So, we'd be replacing data[i] with entry. and this will
             *      leave the deleted key in the inner nodes.
             *
             *      After fix_shortage: We have no way of knowing where data[i] key (which is
             *      same as entry) will end up. It could move down one level by the time
             *      fix_shortage returns.
             *
             *      Therefore it seems like we have no choice but to search for it in
             *      data[ ] AND subset[i]->data[ ]
             * Current Solution: Kind of messy:
             *      After fix_shortage(i+1):
             *      Search for entry in data[ ] and if found, replace it with smallest.
             *      otherwise, search for it in subset[i]->data[ ]
             *          and replace it. that's where the key (entry) will end up after
             *          fix order returns (credit: Jiaruy Li)
             *
             */
            subset[i + 1]->loose_remove(entry);
            fix_shortage(i + 1);
            
            i = first_ge(data, data_count, entry);
            bool found_dup = (i < data_count && data[i] == entry);
            if (found_dup) {
                data[i] = subset[i+1]->smallest_child_by_value();

            }
            else {

                int j = first_ge(subset[i]->data, subset[i]->data_count, entry);
                found_dup = (j < subset[i]->data_count && subset[i]->data[j] == entry);
                if (found_dup) {

                    subset[i]->data[j] = subset[i]->subset[j+1]->smallest_child_by_value();
                }
            }
            //int m = first_ge(data, data_count, entry);
            //bool found_dup = (m < data_count && data[m] == entry);
            //if (found_dup) {
            //    data[m] = subset[i + 1]->smallest_child_by_value();
            //}
            //else {
            //    if (!subset[i]->is_leaf()) {
            //        m = first_ge(subset[i]->data, subset[i]->data_count, entry);
            //        found_dup = (m < subset[i]->data_count && subset[i]->data[m] == entry);
            //        /*           assert(found_dup);*/
            //        if (found_dup) {
            //            subset[i]->data[m] = subset[i]->subset[m + 1]->smallest_child_by_value();
            //        }
            //        else {
            //            if (!subset[i + 1]->is_leaf()) {
            //                m = first_ge(subset[i + 1]->data, subset[i + 1]->data_count, entry);
            //                found_dup = (m < subset[i + 1]->data_count && subset[i + 1]->data[m] == entry);
            //                if (found_dup) {
            //                    cout << "this stage is" << endl;
            //                    cout << *this << endl;
            //                    subset[i + 1]->data[m] = subset[i + 1]->subset[m + 1]->smallest_child_by_value();
            //                }
            //            }
            //        }

            //    }
            //}
            
            //remember. it's possible that data[i] will be gone by the time
            //      fix_shortage returns.
            //key was removed from subset[i+1]:
            //  1. shortage: find entry in data or subset[i+1]->data 
            //              and replace it with subset[i+1]->smallest
            //  2. no shortage: replace data[i] with subset[i+1]->smallest
        }
        // --------------------------------------------------------
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i) {
    /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM,
     *          (borrow from right) transfer / rotate left(i+1)
     * elif child i-1 has more than MINIMUM,
     *          (borrow from left) transfer /rotate right(i-1)
     * elif there is a left sibling,
     *          merge with prev child: merge(i-1)
     * else
     *          merge with next (right) child: merge(i)
     *
     *
     * returns a pointer to the fixed_subset
     */
    const bool debug = false;
    if (subset[i]->data_count < MINIMUM) {
        if ((i < child_count - 1) && (subset[i + 1]->data_count > MINIMUM)) {
            //borrow from right subtree if you can

            transfer_left(i + 1);
            return subset[i];
        }
        //...
       else if ((i - 1 >= 0) && subset[i - 1]->data_count > MINIMUM) {
            //borrow from right subtree if you can

            transfer_right(i - 1);
            return subset[i];
        }

        //CAN CHECK IF THERE'S A RIGHT SIBLING FIRST BEFORE CHECKING IF LEFT SIBLING HERE?
        else if (i + 1 < child_count) {
            merge_with_next_subset(i);
            assert(data_count <= MAXIMUM);
        }
        else {

            merge_with_next_subset(i - 1);
            assert(data_count <= MAXIMUM);
        }

    }
}

template <typename T>
void BPlusTree<T>::transfer_left(int i) {
    /*
     * --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_left
     * ---------------
     *
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i-1],
     * data[i-1] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50  100]
     *  [  ]         [65 75]       ....
     *   [a]      [b]  [c]  [d]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the future child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *               [65 100]
     *   [50]         [ 75 ]       ....
     * [a]  [b]      [c]  [d]
     *
     * ****This does not belong here:
     * last item of subset[i-1]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
     *          [7 | 10]
     *        /    |     \
     * [( )]<=[(7) | 8]  [10]
     *
     * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
     *        [7 | 10]
     *      /    |     \
     *   [7]    [8]   [10]
     *
     * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
     *
     *        [(8) | 10]
     *      /      |     \
     *   [7]     [(8)]   [10]
     *
     *
     *
     */

    //CORRECT WAY TO CHECK IF NON-LEAF CASE?/ NEED TO CHECK IF ALL SUBSETS HAVE CHILDREN ?
    if (!subset[i-1]->is_leaf()) {
        T temp = data[i - 1];
        delete_item(subset[i]->data, 0, subset[i]->data_count, data[i - 1]);
        //cout << "child count is" << child_count << endl;
        //cout << "the I here is " << i << endl;
        attach_item(subset[i - 1]->data, subset[i - 1]->data_count, temp);
        //NEED TO STILL TRANSFER CHILDREN HERE?
        if (subset[i]->child_count > 0) {
            BPlusTree<T>* temp2;

            delete_item(subset[i]->subset, 0, subset[i]->child_count, temp2);
            //cout << "child " << i << " child count is " << subset[i-1]->child_count << endl;
            attach_item(subset[i - 1]->subset, subset[i - 1]->child_count, temp2);

        }
    }

    else {

        T temp;
        delete_item(subset[i]->data, 0, subset[i]->data_count, temp);
        attach_item(subset[i - 1]->data, subset[i - 1]->data_count, temp);
        data[i - 1] = subset[i]->data[0];

    }


}

template <typename T>
void BPlusTree<T>::transfer_right(int i) {
    /*
     * --------------------------------------------------------------
     * transfer right from subset[i] to subset[i+1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave a 'trace':
     *              data[i] is replaced with subset[i+1]->data[0]
     *              after you borrowed the key,
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_right
     * ---------------
     * (i < child_count - 1) and (subset[i-1]->data_count > MINIMUM)
     * subset[i+ 1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i+1]
     *
     * LAST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the LEFT of subset[i+1]->data
     *
     * i = 1
     *                     [50 100]
     *      [20 30]        [65 75]          [ ]
     *  [..] [..] [..]   [a] [b] [c]        [..]
     *
     *  75 moves up to replace 100 (data[i])
     *  75's child (its last child) moves over to be the (child 0) child of 100
     *  100 moves down to subset[i]->data
     *
     *                     [50 75]
     *      [20 30]          [65]          [100]
     *  [..] [..] [..]     [a] [b]        [c] [..]
     *

     //WTFFFFFFFFFFFFF???? IS BELOW ADDITIONAL CORRECT???????
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(2) calls trnasfer_right(1) so, i = 1 subset[i] is ([7|8])
     *  subset[i+1] is the one being fixed.
     *        [7 | 10]
     *      /    |     \
     * [5|6]->[7 | (8)]=>[()]
     *
     * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
     *        [7 | (10)]
     *      /    |     \
     *   [5|6]->[7]-> [8]
     *
     * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
     *
     *        [7 | 8]
     *      /    |    \
     *   [5|6]->[7]-> [8]
     *
     *
     */

    const bool debug = false;


    if (!subset[i+1]->is_leaf()) {

        T temp = data[i];
        detach_item(subset[i]->data, subset[i]->data_count, data[i]);
        insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
        if (subset[i]->child_count > 0) {
            BPlusTree<T>* temp2;
            detach_item(subset[i]->subset, subset[i]->child_count, temp2);
            insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp2);
        }

    }
    else {
        T temp;
        detach_item(subset[i]->data, subset[i]->data_count, temp);
        insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, temp);
        data[i] = subset[i + 1]->data[0];
    }


}


template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i) {
    /*
     * ----------------------------------------------------------------------
     *  Merge subset[i] with subset [i+1] REMOVE data[i];
     *  non leaf: same as BTree
     *  leaves  : delete but do not bring down data[i]
     * ----------------------------------------------------------------------
     *
     *   1. remove data[i] from this object
     *   2. if not a leaf, append it to child[i]->data:
     *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to
     *          right of subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. if a leaf, point subset[i]->next to temp_ptr->next
     *   6. delete temp ptr
     *
     *
     * non-leaf nodes: (same as BTree)
     * ------------------
     *  i = 0:
     *             [50  100]
     *          /      |     \
     *  [  ]         [75]       ....
     *    |         /    \
     *   [a]      [b]    [c]
     *
     *  bring down data[i], merge it with subset[i] and subset[i+1]:
     *      then, get rid of subset[i+1]
     *             [100]
     *               |
     *            [50|75]       ....
     *            /  |  \
     *         [a]  [b]  [c]
     *
     *
     *
     * leaf node:
     * ----------------
     * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
     *
     *  i = 0 : merge 5 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [()]->  [10]
     *
     *  Delete data[i] (7), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [10]
     *      /      \
     *  [5]->      [10]
     *
     *
     * i = 1 merge 7 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [7]->  [()]
     *
     *  Delete data[i] (10), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [7]
     *      /     \
     *  [5]->      [7]
     *
     *
     *
     *
     */
    const bool debug = false;
    //...


    if (!subset[i]->is_leaf()) {
        T temp;
        delete_item(data, i, data_count, temp);
        //WILL APPEND ITEM PERMANENTLY INTO ARRAY WITH STATIC OBJECT temp?

        attach_item(subset[i]->data, subset[i]->data_count, temp);
        merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
        //OKAY IF MERGE FXN TAKES CARE OF SETTING subset[i+1]'s child_count TO ZERO HERE?
        merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
        BPlusTree<T>* temp2;
        delete_item(subset, i + 1, child_count, temp2);
        delete temp2;
        return subset[i];
    }
    else {
        T throwaway;
        delete_item(data, i, data_count, throwaway);
        //WILL APPEND ITEM PERMANENTLY INTO ARRAY WITH STATIC OBJECT temp?

        merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
        //OKAY IF MERGE FXN TAKES CARE OF SETTING subset[i+1]'s child_count TO ZERO HERE?
        merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
        BPlusTree<T>* temp2;
        delete_item(subset, i + 1, child_count, temp2);
        subset[i]->next = temp2->next;
        delete temp2;
        return subset[i];

    }



}

#endif