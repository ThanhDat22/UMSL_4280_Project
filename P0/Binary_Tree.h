#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "BTNode.h"

#include <cstddef>
#include <string>
#include <stdexcept>
#include <iostream>
#include <algorithm>

using std::max;
using std::istream;
using std::ostream;
using std::string;
using std::stringstream;
using std::istringstream;
using std::invalid_argument;

/** A binary tree class */
template<class T>
class Binary_Tree {
public:
	// Constructors

	// Default constructor:
	Binary_Tree() : root(NULL) {}

	/** Construct a Binary_Tree with two subtrees.
		@param the_data The data at the root
		@param left_child The left subtree
		@param right_child The right subtree
	*/
	Binary_Tree(
		const T& the_data,
		const Binary_Tree<T>& left_child = Binary_Tree(),
		const Binary_Tree<T>& right_child = Binary_Tree()
	) :	root( new BTNode<T>(the_data, left_child.root, right_child.root)) {}

	

	// Destructor
	virtual ~Binary_Tree() {} // Do nothing.

	// Getters: methods that are used  to read the private data fields.
	BTNode<T>* get_root();

	// Setters: methods that are used to modify the private data fields.
	void set_root(BTNode<T>* new_root);

	// Class member functions
	
	Binary_Tree<T> get_left_subtree() const; // Return the left subtree.
	Binary_Tree<T> get_right_subtree() const; // Return the right subtree.
	const T& get_data() const; // Return the data field of the root.
	bool is_null() const; // Indicate that this is the empty tree.
	bool is_leaf() const; // Indicate that this tree is a leaf.
	//virtual string to_string() const; // Return a string representation of this tree.
	static Binary_Tree<T> read_binary_tree(istream& in); // Read a binary tree
	string root_to_string() const; // Return a string representation of the root
	int height() const; // Calculates the height of a binary tree.

	// Traversal throughout the tree
	string pre_order(); // Return a pre-order traversal of the tree
	string post_order(); // Return a post-order traversal of the tree	
	string in_order(); // Return a in-order traversal of the tree

	
	
	
protected:
	// Protected constructor
	/** Construct a Binary_Tree with a given node as the root */
	Binary_Tree(BTNode<T>* new_root) : root(new_root) {}

	// Data Field
	BTNode<T>* root; // Stores pointer points to the root of tree.

private:

	string pre_order(const BTNode<T>*, size_t);
	string post_order(const BTNode<T>*, size_t);
	string in_order(const BTNode<T>*, size_t);

}; // End of Binary_Tree


// Operators

// Overloading the ostream insertion operator.
template<class T>
ostream& operator<<(ostream& out, const Binary_Tree<T>& tree) { return out << tree.to_string(); }

// Overloading the istream extraction operator
template<typename T>
istream& operator>>(istream& in, Binary_Tree<T>& tree) {
	tree = Binary_Tree<T>::read_binary_tree(in);
	return in;
}

// Implementation of member functions

// Getters: methods that are used  to read the private data fields.
template<typename T>
void Binary_Tree<T>::set_root(BTNode<T>* new_root) { root = new_root; }

// Setters: methods that are used to modify the private data fields.
template<typename T>
BTNode<T>* Binary_Tree<T>::get_root() { return root; }

// Accessors

// Return the left-subtree.
template<class T>
Binary_Tree<T> Binary_Tree<T>::get_left_subtree() const {
	if (root == NULL) {	throw invalid_argument("get_left_subtree on empty tree"); }
	return Binary_Tree<T>(root->left);
}

// Return the right-subtree.
template<class T>
Binary_Tree<T> Binary_Tree<T>::get_right_subtree() const {
	if (root == NULL) { throw invalid_argument("get_right_subtree on empty tree"); }
	return Binary_Tree<T>(root->right);
}

/** Return the data field of the root
	@throws invalid_argument if null tree
*/
template<class T>
const T& Binary_Tree<T>::get_data() const {
	if (root == NULL) { throw invalid_argument("get_data on null tree"); }
	return root->data;
}

// Indicate that this is the empty tree.
template<class T>
bool Binary_Tree<T>::is_null() const { return root == NULL; }

// Indicate that this tree is a leaf.
template<class T>
bool Binary_Tree<T>::is_leaf() const {
	if (root != NULL) { return root->left == NULL && root->right == NULL; }
	else { return true; }
}

// Return a string representation of the root
template<class T>
string Binary_Tree<T>::root_to_string() const {
	return root->to_string();
}

// Calculates the height of a binary tree.
template<typename T>
int Binary_Tree<T>::height() const {
	if (is_null()) { return 0; }
	return 1 + max(get_left_subtree().height(), get_right_subtree().height());
}

// Return a pre-order traversal of the tree
template<class T>
string Binary_Tree<T>::pre_order(const BTNode<T>* local_root, size_t deepth) {
	string result;

	for (size_t i = 0; i < deepth; i++) { result += " "; }
	deepth++;
	if (local_root != NULL) {
		result += local_root->to_string().at(0);
		result += ": " + local_root->to_string();
		result += "\n"; 		

		if (local_root->left != NULL) {	result += pre_order(local_root->left, deepth); }
		if (local_root->right != NULL) { result += pre_order(local_root->right, deepth); }	
	}
	return result;
}
// Wrapper function
template<class T>
string Binary_Tree<T>::pre_order() { return pre_order(root, 0); }

// Return a post-order traversal of the tree
template<class T>
string Binary_Tree<T>::post_order(const BTNode<T>* local_root, size_t deepth) {
	string result;	
	for (size_t i = 0; i < deepth; i++) { result += " "; }
	deepth++;
	if (local_root != NULL) {
		
		if (local_root->left != NULL) { result += post_order(local_root->left, deepth);	}
				
		if (local_root->right != NULL) { result += post_order(local_root->right, deepth); }
		
		
		result += local_root->to_string().at(0);
		
		result += ": " + local_root->to_string();
		
		result += "\n";
		
		
	}
	return result;
}
// Wrapper function
template<class T>
string Binary_Tree<T>::post_order() { return post_order(root, 0); }

// Return an in-order traversal of the tree
template<class T>
string Binary_Tree<T>::in_order(const BTNode<T>* local_root, size_t deepth) {
	string result;
	for (size_t i = 0; i < deepth; i++) { result += " "; }
	deepth++;

	if (local_root != NULL) {
		
		if (local_root->left != NULL) { result += in_order(local_root->left, deepth); }

		
		result += local_root->to_string().at(0);
		result += ": " + local_root->to_string();
		result += "\n";
		

		if (local_root->right != NULL) { result += in_order(local_root->right, deepth);	}
	}
	return result;
}
// Wrapper function
template<class T>
string Binary_Tree<T>::in_order() { return in_order(root, 0); }

//// Return a string representation of this tree.
//template<class T>
//string Binary_Tree<T>::to_string() const {
//	ostringstream oss;
//	if (is_null()) { oss << "NULL\n"; }
//	else {
//		oss << *root << '\n';
//		oss << get_left_subtree().to_string();
//		oss << get_right_subtree().to_string();
//	}
//	return oss.str();
//} 

// Read Binary_Tree
template<class T>
Binary_Tree<T> Binary_Tree<T>::read_binary_tree(istream& in) {
	string next_line;
	getline(in, next_line);
	if (next_line == "NULL") { return Binary_Tree<T>();	}
	else {
		T the_data;
		istringstream ins(next_line);
		ins >> the_data;
		Binary_Tree<T> left = read_binary_tree(in);
		Binary_Tree<T> right = read_binary_tree(in);
		return Binary_Tree<T>(the_data, left, right);
	}
}

//template<class T>
//string Binary_Tree<T>::space(size_t n) {
//	string space = ' ';
//	string result = "";
//	for (size_t i = 0; i < n; i++) {
//		result += space;
//	}
//	return result;
//}

#endif // !BINARY_TREE_H

