
// Purpose:

// Program implements a database that responds to sql-like commands:


// Data Structure:

 

// Program uses Maps and Multimaps based on B+Trees as the primary data structure:

// Individual indices, list of indices, list of keywords etc.

// Program uses binary files for disk storage of database record data

// Program uses text files for storing names of fields, tables, number of records and other information that needs to be remembered from session to session.

// Program uses a state machine (an adjacency matrix implementation of a graph) representing the grammar for command line interpretation.


#include <iostream>
#include "sql.h"
using namespace std;

int main(){


SQL s;
s.run();





}