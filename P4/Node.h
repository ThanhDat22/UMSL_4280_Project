// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-28

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-03

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using std::string;
using std::vector;
using std::ostringstream;
using std::ostream;

class Node {
public:
	// Constructors
	Node(const string & = "", size_t = 0);

	// Getters
	string get_data() const;
	vector<Node> get_children() const;
	size_t get_line_number() const;


	// Setters
	void set_data(const string&);
	void set_line_number(size_t);

	// Member functions
	void add_child(const Node&);
	string to_string() const;
private:
	string data; // Data of the node
	size_t line_number; // Line number of the node
	vector<Node> children; // Children of the node
};

#endif //NODE_H

