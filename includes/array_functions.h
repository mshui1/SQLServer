#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <iostream>
using namespace std;

//return the larger of the two items
//ASK IF CORRECT IN RETURNING UNCOPIED REFERENCE OBJECT
template <class T>
T maximal(const T& a, const T& b) {

    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

//swap the two items
template <class T>
void swap(T& a, T& b) {

    T temp = a;
    a = b;
    b = temp;
}

//return index of the largest item in data
template <class T>
int index_of_maximal(T data[], int n) {
    int max = 0;
    //CHECK IF n-1 IS CORRECT
    for (int i = 0; i < n - 1; i++) {

        if (data[i + 1] > data[i]) {
            max = i + 1;
        }
    }
    return max;
}

template<class T>
bool is_sorted(T data[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (data[i + 1] < data[i]) {
            return false;
        }

    }
    return true;
}

//insert entry into the sorted array
//data with length n
//ASK ABOUT INSERTING DUPLICATES & IF N IS LENGTH OR CAPACITY OF ARRAY & WHAT HAPPENS IF EXCEEDS ARRAY CAPACITY
template <class T>
void ordered_insert(T data[], int& n, T entry) {
    bool found = false;
    for (int i = 0; i < n; i++) {


        if (data[i] > entry) {

            for (int j = n - 1; j >= i; j--) {
                data[j + 1] = data[j];
            }
            data[i] = entry;
            found = true;
            break;

        }

    }
    if (!found) {
        data[n] = entry;
    }
    //WILL N INCREMENT IF ENTRY IS DUPLICATE OR ADDING EXCEEDS CAPACITY?
    n++;
}

//return the first element in data that is
//not less than entry
template <class T>
int first_ge(const T data[], int n, const T& entry) {
    int result = n;
    for (int i = 0; i < n; i++) {

        if (data[i] >= entry) {
            result = i;
            break;
        }
    }
    return result;
}

//append entry to the right of data
//ASK ABOUT INSERTING DUPLICATES & IF N IS LENGTH OR CAPACITY OF ARRAY & WHAT HAPPENS IF EXCEEDS ARRAY CAPACITY
template <class T>
void attach_item(T data[], int& n, const T& entry) {
    data[n] = entry;
    //WILL N INCREMENT IF ENTRY IS DUPLICATE OR ADDING EXCEEDS CAPACITY?
    n++;
}

//insert entry at index i in data
//ASK IF N IS LENGTH OR CAPACITY OF ARRAY & WHAT HAPPENS IF EXCEEDS ARRAY CAPACITY
//DOES data[i] & everything greater need to shift over? what happens if doing so exceeds array capacity?
template <class T>
void insert_item(T data[], int i, int& n, T entry) {
    for (int j = n - 1; j >= i; j--) {
        data[j + 1] = data[j];
    }
    data[i] = entry;
    n++;

}

//remove the last element in data and place
//it in entry
//DOES n REPRESENT SIZE OF ARRAY?
template <class T>
void detach_item(T data[], int& n, T& entry) {
    //NEED TO DELETE LAST ELEMENT IN data (data[n-1]?)? (CALL DELETE_ITEM FXN BELOW IF YES?)
    entry = data[n - 1];
    n--;
}

//delete item at index i and place it in entry
//DOES n REPRESENT SIZE OF ARRAY?
//USE "DELETE" KEYWORD TO DELETE ITEM AT INDEX i?
//NEED TO DECREMENT ARRAY SIZE?
//NEED TO SHIFT ALL ELEMENTS GREATER THAN INDEX i to right?
template <class T>
void delete_item(T data[], int i, int& n, T& entry) {
    entry = data[i];
    // delete data[i];
    for (int j = i; j < n; j++) {

        data[j] = data[j + 1];
    }
    n--;


}


//append data2 to the right of data1

template <class T>
void merge(T data1[], int& n1, T data2[], int& n2) {
    for (int i = 0; i < n2; i++) {
        //SHOULD USE ATTACH_ITEM TO DO APPEND HERE?/WILL APPEND PERMANENTLY TO data1 ARRAY HERE IF SO?
        attach_item(data1, n1, data2[i]);
    }
    //SHOULD ZERO OUT n2 HERE?
    n2 = 0;
}

template <class T>
void copy_array(T dest[], const T src[],
    int& dest_size, int src_size) {
    for (int i = 0; i < src_size; i++) {
        dest[i] = src[i];
    }
    dest_size = src_size;


}

template <class T>
void copy_array(T dest[], const T src[], int src_size) {
    for (int i = 0; i < src_size; i++) {
        dest[i] = src[i];
    }


}

//move n/2 elements from the right of data1
//and move to data2
//WHAT HAPPENS IF DATA1 HAS ONLY 1 ITEM?
//IF data1 HAS ODD NUMBER OF ELEMENTS (5), LAST 2 GO INTO data2 AND FIRST 3 REMAIN IN data1?
template <class T>
void split(T data1[], int& n1, T data2[], int& n2) {

    // n2=n1/2;
    // n1=n1-n2;

    copy_array(data2, data1 + (n1 - n1 / 2), n2, n1 / 2);
    n1 = n1 - n2;

}




//print array data
//what is pos used for?
//how supposed to use with btree functions without passing in an ostream parameter?
template <class T>
void print_array(const T data[], int n, int pos = -1) {

    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";
    }
    cout << endl;
}

//item > all data[i]
template <class T>
bool is_gt(const T data[], int n, const T& item) {

    for (int i = 0; i < n; i++) {
        if ( item<=data[i]) {
            return false;
        }
    }
    return true;
}

//item <= all data[i]
template <class T>
bool is_le(const T data[], int n, const T& item) {

    for (int i = 0; i < n; i++) {
        if (item > data[i]) {
            return false;
        }
    }
    return true;
}
//-------------- Vector Extra operators: ---------------------

 //print vector list
 template <typename T>
 ostream& operator <<(ostream& outs, const vector<T>& list) {
    /* for (int i = 0; i < list.size(); i++) {

         outs << list[i] << "-";
     }
     return outs;*/
     for (int i = 0; i < list.size(); i++) {
         outs << list[i] << " ";
     }
     return outs;
 }

 //list.push_back addme
 template <typename T>
 vector<T>& operator +=(vector<T>& list, const T& addme) {
     list.push_back(addme);
     return list;
 }

 //WILL temp PERSIST OUTSIDE FXN?
 template <typename T>
 vector<T> operator +(const vector<T>& lhs, const vector<T>& rhs) {
     
     vector<T> temp = lhs;
     temp.insert(temp.end(), rhs.begin(), rhs.end());
     return temp;
 }
#endif