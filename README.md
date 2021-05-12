# SQLServer

Purpose:

Program implements a database that responds to sql-like commands:

#Data Structure:
Program uses Maps and Multimaps based on B+Trees as the primary data structure:
Individual indices, list of indices, list of keywords etc.
Program uses binary files for disk storage of database record data
Program uses text files for storing names of fields, tables, number of records and other information that
needs to be remembered from session to session.
Program uses a state machine (an adjacency matrix implementation of a graph) representing the
grammar for command line interpretation.


#table Class:
The table class manages a single table’s field list and data. it will provide methods for creating new
tables, retrieving an existing table and responding to select queries.
SQL Class
the SQL class manages all the tables that has been created in this system (this includes previous
sessions). It will provide a command line interface to allow the user to issue sql commands (create,
insert and select)

#Batch Processing:
The SQL class will also provide a batch mode: Constructor will take a file name (a text file) as argument
an will execute these commands within:

create table student fields last, first, major

insert into student values Yang, Bo, “Computer Science”

insert into student values Davis, John, Math

insert into student values Johnson, “Mary Ann”, “Biology”

create table employee fields lname, fname, dept, hireyear

insert into employee values Jackson, David, Finance, 2016

insert into employee values Davidson, “Mary Alice”, complaints, 2012

You will be given a file to test your program for submission and for presentation.

Output: batch processing:

//****************************************************************************
// LOGICAL OPERATORS
//****************************************************************************
//.................
//:OR AND :
//.................
[104] select * from student where fname = Flo or major = CS and age <= 23
Table name: _select_table_343, records: 3
 record fname lname major age company
 0 Flo Yao CS 20 Google
 1 Flo Jackson Math 21 Google
 2 Anna Grace Del Rio CS 22 USAF
SQL: DONE.
[105] select * from employee where last = "Van Gogh" or last = Jackson and salary >= 165000
Table name: _select_table_347, records: 5
 record last first dep salary year
 0 Jackson Flo Math 165000 2017
 1 Jackson Billy Math 170000 2017
 2 Van Gogh Vincent Art 240000 2015
 3 Van Gogh Vincent CS 245000 2015
 4 Van Gogh Jim Bob Phys Ed 230000 2010
Consider the example above:
1. There are lines of text in the batch file that begin with two slashes: // These lines must be printed
back to the screen and otherwise ignored by your SQL class.
2. Number your queries starting from zero.
3. Print the query back to the screen as seen above.
4. output of the select statement must contain the table name (temporary table) and the number of
records. This is done in the Table's insertion operator.
5. output of the select statements must contain the record headings as seen above.
6. make sure the output prints in columns.
7. Each record is numbered starting from zero. These are not the record numbers in the original table,
but the record numbers in the resulting table.
