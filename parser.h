#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include "includes/bptree.h"
#include "includes/map.h"
#include "includes/queue.h"
#include "includes/stk.h"
#include "includes/token.h"
#include "includes/maketable.h"
#include "includes/mmap.h"
#include "includes/stack.h"
#include "includes/record.h"
#include "includes/array_functions.h"
#include "includes/queue.h"
#include "includes/list.h"
using namespace std;

class Parser {

public:
    Parser() {
        make_table(adjacency);
        build_keyword_list();
    };
    Parser(const char* s) {

        make_table(adjacency);
        build_keyword_list();
        set_string(s);

    }

    MMap<string, string> parse_tree() {
        return ptree;
    }

    //return true if in fail state, false if in success state
    bool failed() {
        return fail;
    }
    void set_string(const char* s); //processes characters from command string passed in and stores resulting tokens into input_q  


private:
    
    //parses tokens from input_q through state machine into ptree structure, returns false if ends up in success state, true if in fail state
    bool get_parse_tree(); 
	MMap<string, string> ptree; //stores parsed information from command passed
	Map<string, int> keywords; //lookup table for keywords (string) and their corresponding numerical val (int)
	int adjacency[MAX_ROWS][MAX_COLUMNS]; //state machine for token processing
	Queue<Token> input_q; //holds tokens converted from command passed in through set_string
    bool fail; //true if in state machine in fail state, true otherwise
    //corresponding numerical val to keywords
	enum keys { ZERO, SELECT, STAR, FROM, WHERE, COMMA, QUOTE, OPEN_PAREN, CLOSE_PAREN, EQUAL, GT, LT, AND, OR, INSERT, INTO, VALUES, MAKE, CREATE, TABLE, FIELDS, BATCH, QUIT};
	void make_table(int _table[][MAX_COLUMNS]); //initializes state machine with correct values
    void build_keyword_list(); //initializes keywords lookup table
  
    int get_column(string token); //given a string, returns its corresponding numerical val from keyword lookup table










};

void Parser::set_string(const char* s) {
    assert(input_q.empty());
    ptree.clear();
    fail = false;
    STokenizer stk(s);
    Token t;
    stk >> t;
    while (!stk.done()) {

        // process token here...
        //if token is not a space
        if (t.type() != 2) {
            input_q.push(t);
        }
        
        t = Token();
        stk >> t;

    }
    
    fail = get_parse_tree();
};




void Parser::make_table(int _table[][MAX_COLUMNS]) {
    init_table(_table);
    
    //mark if ending state is success or failure after processing all tokens
    mark_fail(_table, 0);
    mark_fail(_table, 1);
    mark_fail(_table, 2);
    mark_fail(_table, 3);
    mark_fail(_table, 5);
    mark_success(_table, 4);
    mark_fail(_table, 6);
    mark_fail(_table, 7);
    mark_fail(_table, 8);
    mark_fail(_table, 9);
    mark_fail(_table,10 );
    mark_fail(_table,11 );
    mark_fail(_table, 12);
    mark_fail(_table, 13);
    mark_success(_table, 14);
    mark_fail(_table,15 );
    mark_success(_table, 16);
    mark_success(_table, 17);
    mark_fail(_table, 18);
    mark_fail(_table, 19);
    mark_fail(_table, 20);
    mark_success(_table,21 );
    mark_fail(_table, 22);
    mark_fail(_table, 23);
    mark_fail(_table, 24);
    mark_success(_table, 25);
    mark_fail(_table,27 );
    mark_fail(_table, 28);
    mark_fail(_table, 29);
    mark_fail(_table,30 );
    mark_success(_table, 31);
    mark_fail(_table,32 );
    mark_fail(_table, 33);
    mark_fail(_table,34 );
    mark_success(_table, 35);
    mark_fail(_table, 37);
    mark_fail(_table, 38);
    mark_fail(_table,39 );
    mark_fail(_table, 40);
    mark_success(_table, 41);
    mark_fail(_table, 42);
    mark_fail(_table, 50);
    mark_success(_table, 51);
    mark_success(_table, 52);




    //mark cells of state machine with (previous state, tablename, numerical val of current token, and ending state)    
    // token that led to previous state -> current token that will lead to ending state
    mark_cell(0, _table, 1, 1); // start -> select
    mark_cell(1, _table, 2, 2); // select -> *
    mark_cell(1, _table, 30, 2); // select -> sym
    mark_cell(2, _table, 3, 3); // */sym -> from
    mark_cell(3, _table, 30, 4); // from -> sym
    mark_cell(4, _table, 4, 5); // sym - > where
    mark_cell(5, _table, 30, 6); // where -> sym
    mark_cell(5, _table, 7, 7); // where -> (
    mark_cell(2, _table, 5, 1);// sym -> ,
    mark_cell(7, _table, 7, 7); // ( -> (
    mark_cell(7, _table, 30, 8); // ( -> sym
    mark_cell(8, _table, 9, 9); // sym -> =
    mark_cell(8, _table, 10, 10);// sym -> >
    mark_cell(8, _table, 11, 11); //sym -> <
    mark_cell(10, _table, 9, 9); // > -> =
    mark_cell(11, _table, 9, 9); // < -> =
    mark_cell(9, _table, 6, 12); // = -> "
    mark_cell(10, _table, 6, 12); // > -> "
    mark_cell(11, _table, 6, 12); // < -> "
    mark_cell(12, _table, 30, 13); // " -> sym
    mark_cell(13, _table, 30, 13); // sym -> sym
    mark_cell(13, _table, 6, 14); // sym -> "
    mark_cell(14, _table, 12, 15); // " -> and
    mark_cell(14, _table, 13, 15); // " -> or
    mark_cell(14, _table, 8, 16); // " -> )
    mark_cell(15, _table, 7, 7); // and/or -> (
    mark_cell(15, _table, 30, 8); // and/or -> sym
    mark_cell(16, _table, 8, 16); // ) -> )
    mark_cell(16, _table, 12, 15);// ) -> and
    mark_cell(16, _table, 13, 15); // ) -> or
    mark_cell(9, _table, 30, 17); // = -> sym
    mark_cell(10, _table, 30, 17); // > -> sym
    mark_cell(11, _table, 30, 17); // < -> sym
    mark_cell(17, _table, 8, 16); // sym -> )
    mark_cell(17, _table, 12, 15); //sym -> and
    mark_cell(17, _table, 13, 15); //sym -> or
    mark_cell(6, _table, 9,18 ); // sym -> =
    mark_cell(6, _table, 10, 19); //sym -> >
    mark_cell(6, _table, 11, 20); //sym -> <
    mark_cell(18, _table, 30, 21); // = -> sym
    mark_cell(21, _table, 12, 22); // sym -> and
    mark_cell(21, _table, 13, 22); // sym -> or
    mark_cell(22, _table, 30, 6); // and/or -> sym
    mark_cell(18, _table, 6, 23); // = -> "
    mark_cell(23, _table, 30, 24); // " -> sym
    mark_cell(24, _table, 30, 24); // sym -> sym
    mark_cell(24, _table, 6, 25); // sym -> "
    mark_cell(25, _table, 12, 22); // " -> and
    mark_cell(25, _table, 13, 22); // " -> or
    mark_cell(19, _table, 9, 18); // > -> =
    mark_cell(20, _table, 9, 18); // < -> =
    mark_cell(19, _table, 30, 21); // > -> sym
    mark_cell(20, _table, 30, 21); // < -> sym
    mark_cell(19, _table,6 , 23); // > -> "
    mark_cell(20, _table, 6, 23); // < -> "
    mark_cell(22, _table,7 ,7 ); // and/or -> (
    mark_cell(0, _table, 14, 27); // start -> insert
    mark_cell(27, _table, 15, 28); // insert -> into
    mark_cell(28, _table, 30, 29); // into -> sym
    mark_cell(29, _table, 16, 30); // sym -> values
    mark_cell(30, _table, 30, 31); // values -> sym
    mark_cell(31, _table, 5, 32); // sym -> ,
    mark_cell(32, _table, 30, 31); // , -> sym
    mark_cell(30, _table, 6, 33); // values -> "
    mark_cell(33, _table, 30, 34); // " -> sym
    mark_cell(34, _table, 30, 34); // sym -> sym
    mark_cell(34, _table, 6, 35); // sym -> "
    mark_cell(35, _table, 5, 32); // " -> ,
    mark_cell(32, _table, 6, 33); // , -> "
    mark_cell(0, _table, 17, 37); // start -> make
    mark_cell(0, _table, 18, 37);// start -> create
    mark_cell(37, _table, 19, 38); // make/create -> table
    mark_cell(38, _table, 30, 39); // table -> sym
    mark_cell(39, _table, 20, 40); // sym -> fields
    mark_cell(40, _table, 30, 41); // fields -> sym
    mark_cell(41, _table, 5, 42); // sym -> ,
    mark_cell(42, _table, 30, 41); // , -> sym
    mark_cell(0, _table, 21, 50); // start -> batch
    mark_cell(50, _table, 30, 51); // batch -> sym
    mark_cell(51, _table, 30, 51); // sym -> sym
    mark_cell(0, _table, 22, 52); // start -> quit

    
    //when there is a keyword (represented by state of index i) in the name of the batch file, the keyword token is valid and leads to state 51 
    for (int i = 1; i < 23; i++) {

        mark_cell(50, _table, i, 51);

    }
    for (int i = 1; i < 23; i++) {

        mark_cell(51, _table, i, 51);

    }


 }

void Parser::build_keyword_list() {

    keywords["SELECT"] = SELECT;
    keywords["*"] = STAR;
    keywords["FROM"] = FROM;
    keywords["WHERE"] = WHERE;
    keywords[","] = COMMA;
    keywords["\""] = QUOTE;
    keywords["("] = OPEN_PAREN;
    keywords[")"] = CLOSE_PAREN;
    keywords["="] = EQUAL;
    keywords[">"] = GT;
    keywords["<"] = LT;
    keywords["AND"] = AND;
    keywords["OR"] = OR;
    keywords["INSERT"] = INSERT;
    keywords["INTO"] = INTO;
    keywords["VALUES"] = VALUES;
    keywords["MAKE"] = MAKE;
    keywords["CREATE"] = CREATE;
    keywords["TABLE"] = TABLE;
    keywords["FIELDS"] = FIELDS;
    keywords["BATCH"] = BATCH;
    keywords["QUIT"] = QUIT;

}   

int Parser::get_column(string token) {
    //if token is a symbol
    if (!keywords.contains(token)) {
        return 30;
    }
    else {

        return keywords[token];
    }

}

bool Parser::get_parse_tree() {
    string token;
    //need to keep track of actual Token object (in addition to additional string token) in order to determine its type (alpha, punct, #)
    //type of token will be used for figuring out spacing between consecutive tokens in select conditions (alpha types proceeded by space while punct not proceeded by space)
    Token actual_token;
    int state = 0;
    string no = "no";
    Queue<string> condition_queue;    //used to separate tokens between quotes
    while (!input_q.empty() && !fail) {
        actual_token = input_q.pop();
        token = actual_token.token_str();

        //stores token with all capitalized letters since keywords are assumed to be capitalized for use elsewhere
        string token_cap = token;
        for (int i = 0; i < token_cap.length(); i++) {
            int c = token_cap[i];
            token_cap[i] = toupper(c);
        }

        //call state machine to get next state based on current state and numerical val of current token
        state = adjacency[state][get_column(token_cap)];
        string yes = "yes";

        switch (state) {

        // start -> select
        case 1:
            if (token != "'" && token != ",") {
                ptree["command"] += token_cap;
            }
            break;

        // select -> */sym
        case 2:
            ptree["fields"] += token;
            break;
        
        // from -> sym
        case 4:
            ptree["table"] += token;
            break;

        // sym - > where
        case 5:
            ptree["where"] += yes;
            break;
        
        // where / and / or -> sym
        case 6:
            ptree["conditions"] += token;
            break;

        // where / and / or / ( -> (
        case 7:
            ptree["conditions"] += token;
            break;

        // and / or / ( -> sym
        case 8:
            ptree["conditions"] += token;
            break;
        
        // > / < / sym -> =
        case 9:
        {
            string last_element = ptree["conditions"].back();
            //if current last element in conditions vector is < or >, combine it with token (= sign) and push back into conditions vector...
            //to form single >= or <= token
            if (last_element == "<" || last_element == ">") {

                ptree["conditions"].pop_back();
                ptree["conditions"] += (last_element + token);

            }
            else {
                ptree["conditions"] += token;
            }
        }
        break;

        // sym -> >
        case 10:
            ptree["conditions"] += token;
            break;
        
        // sym -> <
        case 11:
            ptree["conditions"] += token;
            break;

        // " / sym -> sym
        case 13:
        {
            // if next token in queue is an alpha type, current token is followed by space;
            //else current token followed by no space and will be appended to next token
            Token front = input_q.front();
            if (front.type_string() != "ALPHA") {
                condition_queue.push(token);
            }
            else {
                condition_queue.push(token + " ");
            }
        }
        break;

        // sym -> "
        case 14:
        {
            string temp = "";
            //closing quotes reached so continuously pop from condition queue & add to temp string to form entire value between quotes
            while (!condition_queue.empty()) {
                temp = temp + condition_queue.pop();
            }
            ptree["conditions"] += temp;
        }
        break;

        // " / ) / sym -> and/or
        case 15:
            ptree["conditions"] += token_cap;
            break;

        // " / ) / sym -> )
        case 16:
            ptree["conditions"] += token;
            break;

        // > / < / = -> sym
        case 17:
            ptree["conditions"] += token;
            break;
        
        // > / < -> =
        case 18: {
            string last_element = ptree["conditions"].back();
            //if current last element in conditions vector is < or >, combine it with token (= sign) and push back into conditions vector...
            //to form single >= or <= token
            if (last_element == "<" || last_element == ">") {

                ptree["conditions"].pop_back();
                ptree["conditions"] += (last_element + token);

            }
            else {
                ptree["conditions"] += token;
            }
        }
               break;

        // sym -> >
        case 19:
            ptree["conditions"] += token;
            break;

        //sym -> <
        case 20:
            ptree["conditions"] += token;
            break;

        // > / < / = -> sym
        case 21:
            ptree["conditions"] += token;
            break;
        
        // " / sym -> and/or
        case 22:
            ptree["conditions"] += token_cap;
            break;
        
        // " / sym -> sym
        case 24:
        {
            // if next token in queue is an alpha type, current token is followed by space;
            //else current token followed by no space and will be appended to next token
            if (input_q.front().type_string() != "ALPHA") {
                condition_queue.push(token);
            }
            else {
                condition_queue.push(token + " ");
            }
        }
        break;

        // sym -> "
        case 25:
        {
                string temp = "";
                //closing quote reached so continuously pop from condition queue & add to temp string to form entire value between quotes
                while (!condition_queue.empty()) {
                    temp = temp + condition_queue.pop();
                }
                ptree["conditions"] += temp;
            }
            break;

        // start -> insert
        case 27:
            ptree["command"] += token_cap;
            break;

        // into -> sym
        case 29:
            ptree["table"] += token;
            break;
        
        // , / values -> sym
        case 31:
            ptree["values"] += token;
            break;

        // " / sym -> sym
        case 34:
        {
            // if next token in queue is an alpha type, current token is followed by space;
            //else current token followed by no space and will be appended to next token
            Token front = input_q.front();
            if (front.type_string() != "ALPHA") {
                condition_queue.push(token);
            }
            else {
                condition_queue.push(token + " ");
            }
        }
        break;
        
        // sym -> "
        case 35:
        {
            string temp = "";
            //closing quotes reached so continuously pop from condition queue & add to temp string to form entire value between quotes
            while (!condition_queue.empty()) {
                temp = temp + condition_queue.pop();
            }
            ptree["values"] += temp;
        }
        break;

        // start -> make/create
        case 37:
        {
            string make = "MAKE";
            ptree["command"] += make;
        }
        break;

        // table -> sym
        case 39:
            ptree["table"] += token;
            break;

        // , / fields->sym
        case 41:
            ptree["fields"] += token;
            break;

        // start -> batch
        case 50:
            ptree["command"] += token_cap;
            break;
        
        // batch / sym -> sym
        case 51:
            //continuously append token to continguous name of file 
            if (!ptree["file"].empty()) {
                string last_element = ptree["file"].back();
                ptree["file"].pop_back();
                ptree["file"] += (last_element + token);
            }
            else {
                ptree["file"] += token;
            }
            break;

        // start -> quit
        case 52:
            ptree["command"] += token_cap;
            break;

        case -1:
            fail = true;
            break;
        }


        }
        if (fail) {
            ptree.clear();
            //remove remaining elements in input_q
            while (!input_q.empty()) {
                input_q.pop();
            }
            return true;


        }
        //if ended up in any state but success state
        else if (adjacency[state][0] != 1) {
            ptree.clear();
            //remove remaining elements in input_q
            while (!input_q.empty()) {
                input_q.pop();
            }
            return true;
        }
        else {
        //ended up in success state
            return false;
        }



    }
#endif