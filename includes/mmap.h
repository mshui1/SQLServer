#ifndef MMAP_H
#define MMAP_H
#include <iostream>
#include <vector>
#include "bptree.h"

using namespace std;

template <typename K, typename V>
struct MPair {
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
     //--------------------------------------------------------------------------------
    MPair(const K& k = K()) {
        key = k;
        value_list = vector<V>(0);
    }
    MPair(const K& k, const V& v) {
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist) {
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {

        outs << print_me.key <<": "<< print_me.value_list;
        return outs;

    }

    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {

        if (lhs.key != rhs.key) {

            return true;
        }
        else {
            return false;
        }
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {

        if (lhs.key == rhs.key) {

            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        if (lhs.key < rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        if (lhs.key <= rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        if (lhs.key > rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        if (lhs.key >= rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        /*vector<V> temp = lhs.value_list;*/
        return MPair(lhs.key, lhs.value_list + rhs.value_list);
    }
};

template <typename K, typename V>
class MMap
{
public:

    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator {
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it) {
            _it = it;
        }
        Iterator operator ++(int unused) {
            return _it++;
        }
        Iterator operator ++() {
            return ++_it;
        }
        MPair<K, V> operator *() {
            return *_it;

        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }
    private:
        typename map_base::Iterator _it;

    };
public:
    MMap() {

      
    }

    //  Iterators
    Iterator begin();
    Iterator end();

    //  Capacity
    int size() const {
        return mmap.size();
    }
    bool empty() const;
    //  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);

    //  Modifiers
    void insert(const K& k, const V& v) {
        mmap.insert(MPair<K, V>(k, v));
    }
    void erase(const K& key);
    void clear();


    MPair <K, V> get_biggest();
    //  Operations:
    bool contains(const K& key) const;
    vector<V>& get(const K& key);
    const vector<V>& get(const K& key) const;

    Iterator find(const K& key);

    //SHOULD INSERT key INTO mmap IF DOESN'T EXIST YET?
    int count(const K& key) {

        if (contains(key)) {
            return get(key).size();
        }
        else {
            return 0;
        }
    }
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    Iterator lower_bound(const K& key);

    Iterator upper_bound(const K& key);


    bool is_valid() {
        return mmap.root_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me) {
        outs << print_me.mmap << endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

//SHOULD CREATE A NEW MPAIR FROM key ARGUMENT?
template <typename K, typename V>
bool MMap<K, V>::contains(const K& key) const {
    if (mmap.contains(key)) {
        return true;
    }
    else {
        return false;
    }

}

template<typename K, typename V>
void MMap<K, V>::clear() {

    mmap.clear_tree();
}

template<typename K, typename V>
void MMap<K, V>::erase(const K& key) {
    //DOES BTREE'S REMOVE FXN HAVE TO RETURN BOOL TO DECREMENT key_count?
    mmap.remove(MPair<K, V>(key));
}

//SHOULD THIS FXN AUTOMATICALLY INSERT key IF DOESN'T EXIST, EVEN IF READING OUT ONLY?
template <typename K, typename V>
vector<V>& MMap<K, V>::get(const K& key) {



    return mmap.get(MPair<K, V>(key)).value_list;




}

template <typename K, typename V>
MPair <K, V> MMap<K, V>::get_biggest() {


    return  mmap.bigger_child();



}

template <typename K, typename V>
const vector<V>& MMap<K, V>::get(const K& key) const{

  
    return  mmap.get(MPair<K, V>(key)).value_list;

    

}


//SHOULD THIS FXN AUTOMATICALLY INSERT key IF DOESN'T EXIST, EVEN IF READING OUT ONLY?
template <typename K, typename V>
vector<V>& MMap<K, V>::operator[](const K& key) {

    return get(key);
}


template <typename K, typename V>
 const vector<V>& MMap<K, V>::operator[](const K& key) const{
    

    return get(key);
    
}


 template <typename K, typename V>
 typename MMap<K, V>::Iterator MMap<K, V>::lower_bound(const K& key) {
     ////SHOULD CREATE A NEW PAIR FROM key ARGUMENT AND PASS IN AS ARGUMENT BELOW?
     return mmap.lower_bound(key);
 }


 template <typename K, typename V>
 typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K& key) {
     ////SHOULD CREATE A NEW PAIR FROM key ARGUMENT AND PASS IN AS ARGUMENT BELOW?
     return mmap.upper_bound(key);
 }

 template <typename K, typename V>
 typename MMap<K, V>::Iterator MMap<K, V>::find(const K& key) {
     ////SHOULD CREATE A NEW PAIR FROM key ARGUMENT AND PASS IN AS ARGUMENT BELOW?
     return mmap.find(key);
 }

 template <typename K, typename V>
 typename MMap<K, V>::Iterator MMap<K, V>::begin() {

     return mmap.begin();
 }


 template <typename K, typename V>
 typename MMap<K, V>::Iterator MMap<K, V>::end() {

     return mmap.end();
 }

 template <typename K, typename V>
 bool MMap<K, V>::empty() const {



     return mmap.empty();




 }
#endif