// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-28

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-03

#ifndef TREE_H
#define TREE_H

#include "Node.h"

#include <stack>
using std::stack;

class Tree {

public:
	// Constructors
	Tree(const Node & = Node()); // Default constructor

	// Getters
	Node get_root() const;
	const Node& get_root();

	// Setters
	void set_root(const Node&);

	// Member functions
	string pre_order() const; // Pre-order traversal of the tree wrapper function
private:
	Node root; // Root of the tree

	string pre_order(const Node&, size_t) const; // Pre-order traversal of the tree recursive function
	

};

#endif //TREE_H


