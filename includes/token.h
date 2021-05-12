#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
using namespace std;


class Token
{
public:
    Token() {

        _token = "";
        int type = 0;
    }
    Token(string str, int type) {

        _token = str;
        _type = type;
    }
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type(); //returns type int associated with token
    string type_string(); //returns type associated with token in str format
    string token_str() const; //returns the item stored in token
private:
    string _token;
    int _type;
};


int Token::type() {
    return _type;

}

string Token::token_str() const {
    return _token;
}

string Token::type_string() {
    switch (_type) {
    case 0: return "UNKNOWN";
    case 1: return "ALPHA";
    case 2: return "SPACE";
    case 3: return "NUMBER";
    case 4: return "PUNCTUATION";
    case 5: return "ACCENTED";
    case 6: return "NUMBER";
    default: return "UNKNOWN";
    }

}

ostream& operator <<(ostream& outs, const Token& t) {

    outs << t.token_str();
    return outs;
}
#endif