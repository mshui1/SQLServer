//table Class :
//
//
//
//The table class manages a single table�s field listand data.it will provide methods for creating new tables, retrieving an existing tableand responding to select queries.

#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <string>
#include "includes/record.h"
#include "includes/mmap.h"
#include "includes/map.h"
#include "includes/stack.h"
#include "includes/records.h"
#include "includes/queue.h"
#include <cassert>
using namespace std;

template <typename U>
vector<U> unions(vector<U> v1, vector<U> v2) {


	vector<U> result;
	BPlusTree<U> bt;
	for (int i = 0; i < v1.size(); i++) {

		bt.insert(v1[i]);
		result.push_back(v1[i]);
	}
	for (int i = 0; i < v2.size(); i++) {
		if (!bt.contains(v2[i])) {
			result.push_back(v2[i]);
		}
	}
	return result;
}


template <typename U>
vector<U> intersection(vector<U> v1, vector<U> v2) {
	vector<U> result;
	BPlusTree<U> bt;
	for (int i = 0; i < v1.size(); i++) {
		bt.insert(v1[i]);

	}
	for (int i = 0; i < v2.size(); i++) {
		
		if(bt.contains(v2[i])){

			result.push_back(v2[i]);
		}
	}
	return result;
}




class Table {

public:

	Table() {
		file = new fstream();
	};

	Table(const string& name) {

		_name = name;
		last_record_no = 0;
		_file_name = _name + ".bin";
		file = new fstream();
		open_fileRW(*file, _file_name.c_str());
		
		Record r;
		r.read(*file, 0);
		//get list of table's field names from 1st record in binary file 
		_field_list = r.get_record();

		//initalize indices vectors with empty mmaps for every field of table
		for (int i = 0; i < _field_list.size(); i++) {

			_indices.emplace_back(MMap < string, long>());
		}

		//initialize field_map lookup map with numerical position of field name in table's field names list 
		for (int i = 0; i < _field_list.size(); i++) {
			field_map[_field_list[i]] = i;
		}

		long recno = 1;
		r.read(*file, recno);
		while (!(*file).eof()) {

			//get list of field values at recno
			vectorstr field_list = r.get_record();

			//add recno into indices (at field name with that field value from index i)
			for (int i = 0; i < field_list.size(); i++) {

				_indices[i][field_list[i]] += recno;

			}
			recno++;
			r.read(*file, recno);
		}
		last_record_no = int(recno);
		(*file).close();


	}

	Table(string name, vectorstr field_list) {
		
		
		_name = name;
		last_record_no = 0;
		file = new fstream();
		_file_name = _name + ".bin";
		_field_list = field_list;
		
		open_fileW(*file, _file_name.c_str());
		Record rec(_field_list);
		//write list of field names passed in into first line of binary file
		rec.write(*file);
		(*file).close();

		//initalize indices vectors with empty mmaps for every field of table
		for (int i = 0; i < field_list.size(); i++) {
	
			_indices.emplace_back(MMap < string, long>());
		}

		//initialize field_map lookup map with numerical position of field name in table's field names list 
		for (int i = 0; i < field_list.size(); i++) {
			field_map[field_list[i]] = i;
		}

	}

	

	friend ostream& operator<<(ostream& outs, Table& t);
	void insert_into(vectorstr field_list);
	Table select_where(vectorstr fields, vectorstr conditions); //returns Table with records matching passed in conditinos
	int field_list_size(); //return how many fields table holds



private:
	string _name;
	string _file_name;
	static int serial;
	vectorstr _field_list; //list of field names that table holds
	
	//table's fstream file must be a pointer to a dynamic fstream object (dynamically initalized in CTORs),
	//since C++ doesn't allow implicit copying of fstream objects when system automatically calls default class CTOR
	//as compile-time error was occuring from SQL class (which holds private member var of Map<string, Table>)... 
	//automatically calling Table class default CTOR when Table's private file var was an actual fstream object instead of pointer    
	fstream* file;
	int last_record_no;

	vector<MMap<string, long> > _indices; //holds MMaps (one for each field name col of table) of strings (field values) and longs (recno that have that field value)
	Map<string, int> field_map; //lookup table matching field name to field's column position in table
	vectorstr shunting_yard(vectorstr conditions); //converts infix expression to postfix expression 
	vector<long> rpn(vectorstr shunting_yard); //processes given postfix expression and returns recnos that satisfy all expression's conditions
	vector<long> evaluate(string field, string cond, string op); //returns list of recnos that satisfy condition passed in

};

//write list of field values into bin file first, and update table's indices with returned recno from writing
void Table::insert_into(vectorstr field_list) {
	open_fileRW(*file, _file_name.c_str());
	Record rec(field_list);
	long recno = rec.write(*file);
	(*file).close();
	for (int i = 0; i < field_list.size(); i++) {

		_indices[i][field_list[i]] += recno;

	}
	last_record_no++;


}

ostream& operator<<(ostream& outs,Table& t) {

	outs << "Table name: " << t._name <<" , Number of records: "<<t.last_record_no<< endl << endl;
	outs <<setw(11)<< "record";
Record r;
open_fileRW(*t.file, t._file_name.c_str());
int recno = 0;
r.read(*t.file, recno);
while (!(*t.file).eof()) {
	if(recno==0){
		cout << r << endl << endl;
	}
	else {
		cout <<setw(10)<< recno - 1 <<"."<< r << endl;
	}
	recno++;

	r.read(*t.file, recno);

}
(*(t.file)).close();
outs << "\nNumber of records: " << recno-1 << endl;
return outs;
}


int Table::field_list_size() {
	return _field_list.size();
}

Table Table::select_where(vectorstr fields, vectorstr conditions) {
	if (fields[0]=="*") {
		fields = _field_list;
	}
	serial++;
	Table temp(_name+to_string(serial), fields);

	vector<long> records;

	if (!conditions.empty()) {
		records = rpn(shunting_yard(conditions));
	}

	vector<int> filters; //ints corresponding to column positions of field names in original table

	//initialize filters with column position in original table of given field name at index i 
	for (int i = 0; i < fields.size(); i++) {
		if (field_map.contains(fields[i])) {
			filters.push_back(field_map[fields[i]]);
		}
		else {
			filters.push_back(1000);
		}
	}
	
	//get field values from binary file at recno (at index i), filter list of values for desired field names, and insert result into temp table
	if (!conditions.empty()) {
		for (int i = 0; i < records.size(); i++) {
			vectorstr field_list;
			Record r;
			open_fileRW(*file, _file_name.c_str());
			r.read(*file, records[i]);
			(*file).close();
			field_list = r.filter(filters);
			temp.insert_into(field_list);

		}
	}
	else {

		vectorstr field_list;
		Record r;
		open_fileRW((*file), _file_name.c_str());
		int recno = 1;
		r.read(*file, recno);
		while (!(*file).eof()) {
			field_list = r.filter(filters);
			temp.insert_into(field_list);
			recno++;

			r.read(*file, recno);

		}

		(*file).close();



	}

	return temp;



}


vectorstr Table::shunting_yard(vectorstr conditions) {
	Queue<string> condition_queue;
	vectorstr answer;
	Map<string, int> precedence;
	Stack<string> ops;
	precedence["="] = 2;
	precedence["<"] = 2;
	precedence[">"] = 2;
	precedence[">="] = 2;
	precedence["<="] = 2;
	precedence["AND"] = 1;
	precedence["OR"] = 0;


	for (int i = 0; i < conditions.size(); i++) {
		condition_queue.push(conditions[i]);

	}
	string token;
	while (!condition_queue.empty()) {

		token = condition_queue.pop();
		if (!precedence.contains(token) && token.compare("(") != 0 && token.compare(")") != 0) {
			answer.push_back(token);

		}
		else if (token.compare("(") == 0) {
			ops.push(token);
		}
		else if (token.compare(")") == 0) {
			string temp = ops.pop();
			while (temp.compare("(") != 0) {
				answer.push_back(temp);
				temp = ops.pop();
			}
		}
		else {
			while (!ops.empty() && ops.top().compare("(") != 0 && precedence[ops.top()] >= precedence[token]) {
				answer.push_back(ops.pop());

			}
			ops.push(token);
		}


	}
	while (!ops.empty()) {

		answer.push_back(ops.pop());
	}

	return answer;
}



vector<long> Table::rpn(vectorstr shunting_yard) {
	Stack<string> str; //holds symbols (!= operators)
	Stack<vector<long>> answer;
	Map<string, int> ops; //lookup table for operators

	ops["="] = 2;
	ops["<"] = 2;
	ops[">"] = 2;
	ops[">="] = 2;
	ops["<="] = 2;
	ops["AND"] = 1;
	ops["OR"] = 0;

	for (int i = 0; i < shunting_yard.size(); i++) {
		if (!ops.contains(shunting_yard[i])) {
			str.push(shunting_yard[i]);
		}
		else if (ops[shunting_yard[i]] == 2) {
			answer.push(evaluate( str.pop(), str.pop(), shunting_yard[i] ));
		}
		else if (ops[shunting_yard[i]] == 1) {
			answer.push(intersection(answer.pop(), answer.pop()));
		}
		else {
			answer.push(unions(answer.pop(), answer.pop()));
		}

	}
	vector<long> temp_answer = answer.pop();
	assert(answer.empty());
	return temp_answer;
}


	vector<long> Table::evaluate(string field, string cond, string op) {
		//return empty vector of no recnos if field name of condition passed in doesn't exist in table...
		//in order to not change field_map (since [] op. of maps changes map as soon as called even if lookup value doesn't exist in map)
		if (!field_map.contains(field)) {
			return vector<long>();
		}
		
		Map<string, int> ops; //lookup table for operators used in switch statement below
		vector<long> answer;
		ops["="] = 1;
		ops["<"] = 2;
		ops[">"] = 3;
		ops[">="] = 4;
		ops["<="] = 5;

		int temp = ops[op];

		switch (temp) {

		case 1: {
			answer = _indices[field_map[field]][cond];
		}
			break;
		case 2:
		{
			MMap<string, long>::Iterator it = _indices[field_map[field]].begin();
			while (it != _indices[field_map[field]].end() && (*(it)).key < cond) {
				answer = answer + (*(it)).value_list;
				it++;

			}
		}
			break;
		case 5:
		{
			MMap<string, long>::Iterator it = _indices[field_map[field]].begin();
			while (it != _indices[field_map[field]].end() && (*(it)).key <= cond) {
				answer = answer + (*(it)).value_list;
				it++;

			}
		}
			break;
		case 4:
			
		{MMap<string, long>::Iterator it = _indices[field_map[field]].lower_bound(cond);
		if ((*(it)).key >= cond) {
			while (it != _indices[field_map[field]].end()) {
				answer = answer + (*(it)).value_list;
				it++;
			}
		}}
			break;
		case 3:
		{
			MMap<string, long>::Iterator it = _indices[field_map[field]].upper_bound(cond);
			if ((*(it)).key > cond) {
				while (it != _indices[field_map[field]].end()) {
					answer = answer + (*(it)).value_list;
					it++;
				}
			}}
			break;



		}
		return answer;
	}

int Table::serial = 0;



#endif