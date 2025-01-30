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
	Node(const string & = "");

	// Getters
	string get_data() const;
	vector<Node> get_children() const;

	// Setters
	void set_data(const string&);

	// Member functions
	void add_child(const Node&);
	string to_string() const;
private:
	string data; // Data of the node
	vector<Node> children; // Children of the node
};

#endif //NODE_H

