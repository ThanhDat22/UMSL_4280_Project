// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-28

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-03

#include "Node.h"

// Constructors
Node::Node(const string& data, size_t line_number) : data(data), line_number(line_number) {}

// Getters
string Node::get_data() const {
	return data;
}

vector<Node> Node::get_children() const {
	return children;
}

size_t Node::get_line_number() const {
	return line_number;
}

// Setters
void Node::set_data(const string& data) {
	this->data = data;
}

void Node::set_line_number(size_t line_number) {
	this->line_number = line_number;
}

// Member functions

/** Adds a child node to the children vector.
	@param child: the child Node to add.
*/
void Node::add_child(const Node& child) {
	children.push_back(child);
}

/** Converts the Node to a string.
	@return: the string representation of the Node.
*/
string Node::to_string() const {
	ostringstream oss;
	oss << data;
	return oss.str();
}

// Overloaded operator<<
ostream& operator<<(ostream& os, const Node& node) {
	os << node.to_string();
	return os;
}

