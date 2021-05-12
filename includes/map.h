#ifndef MAP_H
#define MAP_H
#include "bptree.h"
template <typename K, typename V>
struct Pair {
    K key;
    V value;

    //WHERE (ELSE) IS DEFAULT V() CONSTRUCTOR AUTOMATICALLY CALLED? / WHEN INSERTING? / GETTING? / []?
    Pair(const K& k = K(), const V& v =V()) {
        key = k;
        //cout<<"key is"<<key<<endl;
        value = v;
        //cout<<"value is"<<value<<endl;
    }
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me) {
        outs << print_me.key <<": "<< print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        if (lhs.key == rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        if (lhs.key != rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        if (lhs.key < rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        if (lhs.key > rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        if (lhs.key <= rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        if (lhs.key >= rhs.key) {
            return true;
        }
        else {
            return false;
        }
    }

    //WHY NEED THIS FXN IF PAIRS CAN BE OVERWRITTEN WITHOUT IT IN LOOSE INSERT FXN?
    //DO MAP/MULTIMAP AND/OR VECTOR CLASSES ALSO IMPLEMENT OWN + / += OPERATORS?
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {


        return rhs;
    }
};


template <typename K, typename V>
class Map
{
public:
     typedef BPlusTree<Pair<K, V> > map_base;
     class Iterator{
     public:
         friend class Map;
         Iterator(typename map_base::Iterator it) {
             _it = it;
         }
         Iterator operator ++(int unused) {
             return _it++;
         }
         Iterator operator ++() {
             return ++_it;
         }
         Pair<K, V> operator *() {
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

//CONSTRUCTOR HERE NEEDS TO INITIALIZE/DO ANYTHING WITH MAP PRIVATE VARIABLE?
//HOW TO SET PRIVATE map VARIABLE'S dups to TRUE FROM MAP CLASS?
    Map() {


    };
    //  Iterators
         Iterator begin();
         Iterator end();

    //  Capacity
    //SHOULD CALL BTREE CLASS'S size() FXN? OR RETURN key_count PRIVATE MEMBER VARIABLE?
    int size() const {
        return map.size();
    }
    bool empty() const {
        return map.empty();
    }

    //  Element Access
    V& operator[](const K& key);
    const V& operator[](const K& key) const;
    V& at(const K& key);
    const V& at(const K& key) const;


    //  Modifiers
    void insert(const K& k, const V& v) {
        //DOES BTREE INSERT FXN RETURN A BOOL TO INCREMENT key_count?
        map.insert(Pair<K, V>(k, v));
    }
    void erase(const K& key);
    void clear();
    V& get(const K& key);
    const V& get(const K& key) const;

    //  Operations:
         Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool is_valid() { return map.root_valid(); }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me) {
        outs << print_me.map << endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};

//WILL CALLING THIS FUNCTION INCREMENT key_count?
//SHOULDN'T RETURNED V BE A REFERENCE IN ORDER TO BE USED IN [] OPERATOR FXN?
template <typename K, typename V>
V& Map<K, V>::get(const K& key) {



    return map.get(Pair<K, V>(key)).value;




}

//WILL CALLING THIS FUNCTION INCREMENT key_count?
//SHOULD THIS FXN AUTOMATICALLY INSERT key IF DOESN'T EXIST, EVEN IF READING OUT ONLY?
template <typename K, typename V>
V& Map<K, V>::operator[](const K& key) {

    return get(key);
}

template <typename K, typename V>
const V& Map<K, V>::operator[](const K& key) const {

    return get(key);
}

//WILL CALLING THIS FUNCTION INCREMENT key_count?
//WILL CALLING THIS FUNCTION CREATE NEW KEY IF NOT ALREADY IN BTREE?
template <typename K, typename V>
V& Map<K, V>::at(const K& key) {
    return get(key);
}

template <typename K, typename V>
const V& Map<K, V>::at(const K& key) const {
    return get(key);
}


template<typename K, typename V>
void Map<K, V>::erase(const K& key) {
    //DOES BTREE'S REMOVE FXN HAVE TO RETURN BOOL TO DECREMENT key_count?
    map.remove(Pair<K, V>(key));
}


template <typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V>& target) const {
    if (map.contains(target)) {
        return true;
    }
    else {
        return false;
    }

}

template<typename K, typename V>
void Map<K, V>::clear() {

    map.clear_tree();
}

template <typename K,typename V>
typename Map<K,V>:: Iterator Map<K, V>::find(const K& key) {
    ////SHOULD CREATE A NEW PAIR FROM key ARGUMENT AND PASS IN AS ARGUMENT BELOW?
    return map.find(key);
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin() {
 
    return map.begin();
}


template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end() {

    return map.end();
}
#endif