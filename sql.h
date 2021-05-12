//the SQL class manages all the tables that has been created in this system (this includes previous sessions). It will provide a command line interface to allow the user to issue sql commands (create, insert and select)
//The SQL class will also provide a batch mode: Constructor will take a file name (a text file) as argument an will execute these commands within:

#ifndef SQL_H
#define SQL_H

#include <iostream>
#include "includes/record.h"
#include "includes/map.h"
#include "parser.h"
#include "includes/records.h"
#include <cstring>
#include "includes/bptree.h"
#include "table.h"
#include "includes/stack.h"
#include "includes/mmap.h"
#include <fstream>
#include <string>
#include "includes/queue.h"
#include "includes/array_functions.h"
#include <cassert>
using namespace std;

class SQL {

public:

	SQL() {
		//opens text file where names of tables are saved, creating an empty one if doesn't exit
		myfile.open("tables.txt");

		initialize_tables();

	};
	
	//SQL class's main run command that continuously reads input from user until they until "quit"
	void run();

private:

	fstream myfile;
	Parser p; //SQL's Parser object 
	void update_table_names(); //called every time a new table is made/created to save name of new table in text file
	Map<string, Table> tables; //stores tables that SQL class manages
	
	void initialize_tables(); //initializes all tables whose names are stored in text file where name of tables are saved

	//runs command passed in, including batch 
	//return true if will terminate execution of main sql.run() loop, false otherwise
	 bool run_command(string command, int& query_no, bool batch_mode=false);

};

void SQL::update_table_names() {
	//empties entire text file of table names at start of writing
	myfile.open("tables.txt", std::fstream::out | std::fstream::trunc);

	//iterate through tables stored in class and write their names line by line into text file
	Map<string, Table>::Iterator it = tables.begin();
	while (it != tables.end()) {
		myfile << (*it).key << '\n';
		it++;
	}
	myfile.close();

}

void SQL::run() {
	string command;
	cout << "Enter \"QUIT\" to exit the program" << endl;
	while (true) {

		cout << "\n"<< "> ";
		getline(cin, command);
		int query_no = 0;
		if (!run_command(command, query_no, false)) {
			break;
		}
	}
	cout<<"----------------------------------------------"<<endl;
	cout<<"EXITING SQL RUN"<<endl;
	cout<<"----------------------------------------------"<<endl;
}

//skips table names that are mistakenly in text file where table names are saved but have no saved corresponding binary files
void SQL:: initialize_tables() {
		 string table_name;
		 cout << "------SQL Tables I Manage : --------" << endl;
		 while (myfile >> table_name)
		 {
			 if (file_exists((table_name + ".bin").c_str())) {
				 cout << table_name << endl;
				 tables[table_name] = Table(table_name);
			 }
			 else{
				 cout<<"File"<<table_name<<"doesn't exist"<<endl;
			 }
		 }
		 cout << "---------------------------" << endl;
		 myfile.close();
	 };

//batch_mode bool argument for query numbering (true for batch mode requiring query numbering, false for interactive)
bool SQL:: run_command(string command, int& query_no, bool batch_mode) {
		 if (command.empty()) {
			 return true;
		 }
		 p.set_string(command.c_str());
		 //command passed in failed to be correctly parsed
		 if (p.failed()) {
			 cout << "Invalid SQL command entered" << endl;
			 cout << "Invalid line is: " << command << endl;
			 return true;
		 }

		 MMap<string, string> ptree = p.parse_tree();
		 
		 //user wants to terminate execution of sql.run() loop
		 if (ptree["command"][0] == "QUIT") {

			 return false;
		 }

		 if (batch_mode) {
			 cout << "[" << query_no << "] " << command<< endl << endl;
		 }
		 query_no++;

		 if (ptree["command"][0] == "SELECT") {
			 string table_name = ptree["table"][0];
			 if (!tables.contains(table_name)) {
				 cout << "Table " << table_name << " does not exist" << endl;
			 }
			 else {
				 Table temp = tables[table_name].select_where(ptree["fields"], ptree["conditions"]);
				 cout << temp << endl;
			 }

		 }

		 else if (ptree["command"][0] == "MAKE") {
			 string table_name = ptree["table"][0];
			 
			 //doesn't allow user to create a table with name that already exists
			 if (tables.contains(table_name)) {
				 cout << "Table " << table_name << " already exists!" << endl;

			 }
			 else {
				 tables[table_name] = Table(table_name, ptree["fields"]);
				 
				 update_table_names(); //update text file where names of tables are saved
				 
				 cout << "Table " << table_name << " created. cols: " << ptree["fields"] << endl << endl;

			 }

		 }

		 else if (ptree["command"][0] == "INSERT") {
			 string table_name = ptree["table"][0];
			 
			 //cannot insert into a non-existent table
			 if (!tables.contains(table_name)) {
				 cout << "Table " << table_name << " does not exist" << endl;
			 }

			 //number of inserted field values must match number of table's field names
			 else if (ptree["values"].size() != tables[table_name].field_list_size()) {
				 cout << "Number of entered field values doesn't match " << table_name << "\'s number of fields" << endl;
			 }

			 else {
				 tables[table_name].insert_into(ptree["values"]);
				 cout << "SQL::run: inserted." << endl << endl;
			 }
		 }

		 if (ptree["command"][0] == "BATCH") {
			 
			 //do nothing if file name after batch command doesn't exist
			 if (!file_exists(ptree["file"][0].c_str())) {

				 return true;
			 }
			 else {
				 int query_no = 0;
				 ifstream batch_file;
				 batch_file.open(ptree["file"][0].c_str());
				 string line;
				 while (!batch_file.eof()) {
					 getline(batch_file, line);
					 //if line starts with slash, reprint the line
					 if (line.c_str()[0] == '/') {

						 cout << line << endl;
						 continue;
					 }
					 else if (line.empty()) {
						 cout << endl;
						 continue;
					 }
					 else {
						 
						 //recursively call this run_command function on the command with batch mode arg as true to print query numbering in results 
						 run_command(line, query_no, true);

					 }



					 cout << "SQL DONE" << endl;

				 }
				 cout << "--------------------End of Batch Processing-------------------------" << endl;

			 }

		 }
		 return true;



	 }
#endif 