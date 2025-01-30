#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "Binary_Tree.h"

template<class T>
class Binary_Search_Tree : public Binary_Tree<T> {
public:
	// Constructor
	Binary_Search_Tree() : Binary_Tree<T>() {}

	// Member functions
	virtual bool insert(const T&); // Insert an item into the tree.
	virtual bool erase(const T&); // Remove an item from the tree.
	const T* find(const T&) const; // Determine whether an item is in the tree.
private:
	virtual bool insert(BTNode<T>*&, const T&); // Insert an item into the tree.
	virtual bool erase(BTNode<T>*&, const T&); // Remove an item from the tree.
	const T* find(BTNode<T>*, const T&) const; // Determine whether an item is in the tree.
	virtual void replace_parent(BTNode<T>*&, BTNode<T>*&); // Find a replacement for a node that is being deleted.

};

// Implementation of member functions

/** Insert an item into the tree.
      @param item The item to be inserted
      @return {true} if the item was not already in the tree, {false} otherwise
  */
template<class T>
bool Binary_Search_Tree<T>::insert( const T& item) {
    return insert(this->root, item);
}

/** Insert an item into the tree.
    @param local_root A reference to the current root
    @param item The item to be inserted
    @return {true} if the item was not already in the tree, {false} otherwise
  */
template<class T>
bool Binary_Search_Tree<T>::insert( BTNode<T>*& local_root, const T& item) {
    if (local_root == NULL) {
        local_root = new BTNode<T>(item);
        return true;
    } else {
        if (item < local_root->data) { return insert(local_root->left, item); }
        else if (local_root->data < item) { return insert(local_root->right, item); }
        else { return false; }
		}
}

/** Remove an item from the tree.
    @param item The item to be removed
    @return {true} if the item was in the tree, {false} otherwise
 */
template<class T>
bool Binary_Search_Tree<T>::erase( const T& item) {
    return erase(this->root, item);
}

/** Remove an item from the tree.
    @param local_root A reference to the current root
    @param item The item to be removed
    @return {true} if the item was in the tree, {false} otherwise
 */
template<class T>
bool Binary_Search_Tree<T>::erase( BTNode<T>*& local_root, const T& item) {
    if (local_root == NULL) { return false; }
    else {
        if (item < local_root->data) { return erase(local_root->left, item); }
        else if (local_root->data < item) { return erase(local_root->right, item); }
        else { // Found item
            BTNode<T>* old_root = local_root;
            if (local_root->left == NULL) { local_root = local_root->right; }
            else if (local_root->right == NULL) {
                local_root->left = local_root->right;
                local_root = local_root->left;
            }
            else {
                replace_parent(old_root, old_root->left);
            }
            delete old_root;
            return true;
        }
    }
}

/** Find a replacement for a node that is being deleted.
      This function finds the rightmost local root that
      does not have a right child. The data in this local_root
      replaces the data in old_root. The pointer to local_root
      is then saved in old_root and local_root is replaced
      by its left child.
      @param old_root Reference to the pointer to old parent
      @param local_root Reference to the pointer to local root
  */
template<class T> 
void Binary_Search_Tree<T>::replace_parent(BTNode<T>*& old_root, BTNode<T>*& local_root) {
    if (local_root->right != NULL) { replace_parent(old_root, local_root->right); }
    else {
        old_root->data = local_root->data;
        old_root = local_root;
        local_root = local_root->left;
    }
}

/** Determine whether an item is in the tree.
    @param item The item sought
    @return A const pointer to the item if in the tree, or NULL if not
  */
template<class T>
const T* Binary_Search_Tree<T>::find( const T& item) const { 
    return find(this->root, item); 
}

/** Determine whether an item is in the tree.
    @param local_root A reference to the current root
    @param target The item sought
    @return A const pointer to the item in the tree
 */
template<class T>
const T* Binary_Search_Tree<T>::find( BTNode<T>* local_root, const T& target) const {
    if (local_root == NULL) { return NULL; }
    if (target < local_root->data) { return find(local_root->left, target); }
    else if (local_root->data < target) { return find(local_root->right, target); }
    else { return &(local_root->data); }
}


#endif // !BINARY_SEARCH_TREE_H

