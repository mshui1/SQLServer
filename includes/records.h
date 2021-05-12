#ifndef RECORDS_H
#define RECORDS_H
#include <iostream>
using namespace std;

template <typename T>
struct record
{
    int _key;
    T _value;

    record(int k = 0, T v = T()) :_key(k), _value(v) {


    };

    template <typename TT>
    friend bool operator ==(const record<TT>& left, const record<TT>& right);

    template <typename TT>
    friend bool operator <(const record<TT>& left, const record<TT>& right);

    template <typename TT>
    friend bool operator >(const record<TT>& left, const record<TT>& right);

    template <typename TT>
    friend ostream& operator <<(ostream& outs, const record<TT>& print_me);
};

template <typename TT>
bool operator ==(const record<TT>& left, const record<TT>& right) {


    if (left._key == right._key)
        return true;


    else {
        return false;
    }
}

template <typename TT>
bool operator <(const record<TT>& left, const record<TT>& right) {

    if (left._key < right._key)
        return true;


    else {
        return false;
    }
}

template <typename TT>
bool operator >(const record<TT>& left, const record<TT>& right) {

    if (left._key > right._key)
        return true;


    else {
        return false;
    }
}

template <typename TT>
ostream& operator <<(ostream& outs, const record<TT>& print_me) {

    outs << print_me._key << ":" << print_me._value;
    return outs;
}



#endif