
#ifndef FTK_H
#define FTK_H

#include <iostream>
#include "token.h"
#include <fstream>
#include "stk.h"
using namespace std;

const int MAX_BLOCK = MAX_BUFFER;
class FTokenizer
{
public:
    
    //constructor reads in file and sets up STokenizer member
    FTokenizer(const char* fname) {
       
        _f.open(fname);
        if (_f.fail()) {
            cout << "file did not open!" << endl;
        }
        char buffer[MAX_BLOCK+1]; //buffer has at least MAX + 1 capacity allocated.
        _f.read(buffer, MAX_BLOCK);
        buffer[_f.gcount()] = NULL; //sets last index of buffer to null character
        //checks if buffer is empty
        if (_f.gcount() != 0) {

            _stk = STokenizer(buffer);
            _more = true;



        }
        else {
            _more = false;
        }

        

        
        }

    
    Token next_token();
    bool more();        //returns the current value of _more
    int pos();          //returns the value of _pos
    int block_pos();     //returns the value of _blockPos
    friend FTokenizer& operator >> (FTokenizer& f, Token& t) {
        f._stk >> t;
        if (!f._stk.done()) {
            return f;
        }
        //if finished reading previous block, read next block and extract its first token
        else {
            f.get_new_block();

            f._stk >> t;
            return f;

        }

    }
private:
    bool get_new_block(); //gets the new block from the file
    ifstream _f;   //file being tokenized
    STokenizer _stk;     //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
};

bool FTokenizer::get_new_block() {

    char buffer[MAX_BLOCK+1];
    _f.read(buffer, MAX_BLOCK);
    buffer[_f.gcount()] = NULL;
    if (_f.gcount() != 0) {

        _stk = STokenizer(buffer);
        return true;



    }
    else {
        _more = false;
        return false;
    }
}
bool FTokenizer::more() {
    return _more;
}
int FTokenizer::pos() {

    return _pos;
}

int FTokenizer::block_pos() {

    return _blockPos;
}


#endif