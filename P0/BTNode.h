#ifndef BTNODE_H
#define BTNODE_H

#include <iostream>
#include <sstream>
#include <string>

using std::ostream;
using std::ostringstream;
using std::string;

template<class T>
struct BTNode {
    // Data fields
    T data;  // Stores some data in the node.
    BTNode<T>* left; // Stores pointers to the left child.
    BTNode<T> *right;  // Stores pointers to the right child.

    // Constructor
    BTNode(const T&, BTNode<T>* = NULL, BTNode<T>* = NULL);

    // Destructor
    virtual ~BTNode();  // To avoid warning messages

    // Member function
    string to_string() const;  // Returns a string representing the data stored in the node.
};

// Constructor
template<class T>
BTNode<T>::BTNode(const T& data, BTNode<T>* left, BTNode<T>* right) :
    data(data), left(left), right(right) {}

// Destructor
template<class T>
BTNode<T>::~BTNode() {}

/** Returns a string representing the data stored in the node.
    @return: a string representing the data stored in the node
*/
template<class T>
string BTNode<T>::to_string() const {
    ostringstream oss;
    oss << data;
    return oss.str();
}

// Stream insertion operator
template<class T>
ostream& operator << (ostream& out, const BTNode<T>* node) { return out << node->to_string(); }

#endif // !BTNODE_H
