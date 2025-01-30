// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-28

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-03

#include "Tree.h"

// Constructors
Tree::Tree(const Node& root) : root(root) {}

// Getters

// Returns the root of the tree.
Node Tree::get_root() const {
	return root;
}

// Returns the root of the tree.
const Node& Tree::get_root() {
	return root;
}

// Setters

// Sets the root of the tree.
void Tree::set_root(const Node& root) {
	this->root = root;
}

// Member functions

/** Pre-order traversal of the tree.
	@return: the pre-order traversal of the tree.
*/
string Tree::pre_order(const Node& node, size_t depth ) const {

	if (root.get_data().empty()) {
		return "";
	}

	ostringstream result;
	result << string(depth * 2, ' ') << node.get_data() << '\n';

	// Recursively traverse the children of the node
	const vector<Node> children = node.get_children();
	for (size_t i = 0; i < children.size(); i++) {
		result << pre_order(children[i], depth + 1);
	}
	
	return result.str();
}

// Wrapper function for pre_order
string Tree::pre_order() const {
	return pre_order(root, 0);
}

