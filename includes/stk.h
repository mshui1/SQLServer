#ifndef STK_H
#define STK_H
#include <stdio.h>
#include <string.h>
#include "maketable.h"
#include "token.h"

const int MAX_BUFFER = 500;
class STokenizer
{
public:
    STokenizer() {
        _pos = 0;
        make_table(_table);
    }
    //constructor copies the given string to object's buffer
    STokenizer(const char str[]) {
        set_string(str);
        _pos = 0;
        make_table(_table);
    }
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t) {
        s.get_token(0, t);
        return s;

    }

    //set a new string as the input string
    void set_string(const char str[]) {
        strcpy(_buffer, str);

    }

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int start_state, Token& token);
    //---------------------------------
    char _buffer[MAX_BUFFER + 1];       //input string
    int _pos;                 //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};


bool STokenizer::done() {

    if (_pos > strlen(_buffer)) {
        return true;
    }
    else {
        return false;
    }
}

bool STokenizer::more() {

    if (_buffer[_pos] != NULL) {
        return true;
    }
    else {
        return false;
    }
}

bool STokenizer::get_token(int start_state, Token& token) {
    //check to see buffer if is empty
    if (_buffer[_pos] == NULL) {
        _pos++;
        return false;
    }

    //check if first character is outside ASCII table
    if (_buffer[_pos] < 0) {

        string s = "";
        s.push_back(_buffer[_pos]);
        Token tk(s, 5);
        token = tk;
        _pos++;
        return true;
    }

    //save current position in buffer
    int originalposition = _pos;
    int lastsuccessposition = _pos;

    //save value corresponding to char. (at given pos. in buffer) in start state row of table
    int result = _table[start_state][_buffer[_pos]];;
    int successstate = start_state;

    //loop while not at end of buffer and result variable is recognized char. in table
    while (result != -1 && _buffer[_pos] != NULL) {
        //save pos. in buffer & success state (row # in table) if char at pos. results in success state 
        if (_table[result][0] == 1) {
            lastsuccessposition = _pos;
            successstate = result;
        }
        _pos++;
        //save value corresponding to char. (at incremented pos. in buffer) in result row of table
        result = _table[result][_buffer[_pos]];
    }

    //reset pos. in buffer to last pos. whose char. resulted in success state
    _pos = lastsuccessposition + 1;

    //extract substring of buffer and return in token along with last success state
    string sub = _buffer;
    sub = sub.substr(originalposition, (lastsuccessposition + 1) - originalposition);
    Token tk(sub, successstate);
    token = tk;
    return true;

}

void STokenizer::make_table(int _table[][MAX_COLUMNS]) {
    init_table(_table);
    mark_fail(_table, 0);
    mark_success(_table, 1);
    mark_success(_table, 2);
    mark_success(_table, 3);
    mark_success(_table, 4);
    mark_success(_table, 6);
    mark_cells(0, _table, 'A', 'Z', 1);
    mark_cells(1, _table, 'A', 'Z', 1);
    mark_cells(0, _table, 'a', 'z', 1);
    mark_cells(1, _table, 'a', 'z', 1);
    mark_cells(0, _table, '0', '9', 3);
    mark_cells(3, _table, '0', '9', 3);
    mark_cells(3, _table, '.', '.', 6); //if number(s) followed by period, go to row 6 for double
    mark_cells(6, _table, '0', '9', 6); //row 6 for number(s) (ONLY) following period preceded by number(s)- thus double
    mark_cells(0, _table, " \t\r\n", 2);
    mark_cells(2, _table, " \t\r\n", 2); //whitespace
    mark_cells(0, _table, "'_(),-.~`;\"/", 4);
}

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];
#endif

