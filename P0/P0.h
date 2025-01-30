#ifndef P0_H
#define P0_H

#include"Binary_Search_Tree.h"

#include<string>
#include<vector>
#include<iostream>


using std::string;
using std::vector;
using std::cout;
using std::endl;

/**	Indecates whether two string is the same
	@param st1: the first string
	@param st2: the second string
	@return {true} if two string is the same, {false} otherwise
*/
static bool is_equal(string st1, string st2) {
	if (st1.size() != st2.size()) { return false; }
	for (size_t i = 0; i < st1.size(); i++) {
		if (st1.at(i) != st2.at(i)) { return false; }
	}
	return true;
}

/** Remove the duplicate string in vector
	@param vec: the vector of string
	@return the vector of string without dublicated elements.
*/
static vector<string> remove_duplicate(vector<string>& data) {
	for (size_t i = 0; i < data.size() - 1; i++) {
		for (size_t j = i + 1; j < data.size(); j++) {
			if (is_equal(data.at(i), data.at(j))) { data.erase(data.begin() + j); }
		}	
	}
	return data;
}

/*	Groups the strings that the same first letter into a single string
	@param data: the vector of strings that unorder
	@return the vector of strings that every element is a group of words with the same first letter
*/
static vector<string> group_words(vector<string>& data) {
	remove_duplicate(data);
	vector<string> result;
	for (size_t i = 0; i < data.size(); i++) {
		string st = data.at(i) + ' ';
		for (size_t j = i + 1; j < data.size(); j++) {
			if (data.at(i).at(0) == data.at(j).at(0)) {
				st += data.at(j) + " ";
				data.erase(data.begin() + j);
				j--;
			}
		}
		result.push_back(st);
	}
	return result;
}

/**	Build binary search tree
	@param data: the vector of strings.
	@return the Binary_Search_Tree
*/
static Binary_Search_Tree<string> build_tree(vector<string>& data) {
	Binary_Search_Tree<string> tree;
	vector<string> nodes = group_words(data);
	for (size_t i = 0; i < nodes.size(); i++) {
		tree.insert(nodes.at(i));
	}
	return tree;
}

// Display the warning the error
static void exit_error(string s) {
	cout << s << endl;
	exit(1);
}

/*	Split a sentence into worlds
	@param sentence: a sentence or some strings
	return a vector of string
*/
static vector<string> split_sententce(string& sentence) {
	
	stringstream ss(sentence); // Create a stringstream object
	string word;
	vector<string> words; // Vector to store the words

	// Extract words from the sentence
	while (ss >> word) {		
		words.push_back(word); // Add the word to the vector
	}
	return words;

}

//Convert every character in a string to lower case letter
static string to_lower_case(string& s) {
	string result = "";
	for (size_t i = 0; i < s.size(); i++) {
		result += tolower(s[i]);
	}
	return result;
}

// Check whether there is only letter in a string
static bool is_all_letter(string& s) {
	for (size_t i = 0; i < s.size(); i++) {
		if (isalpha(s[i]) == 0) { return false; }
	}
	return true;
}

/*	Saperate file name from the extension
	@param: the filename included extension
	@return the filename without extension
*/
static string get_file_name(string& somefile) {
	string result = "";
	
	for (size_t i = 0; i < somefile.size(); i++) {
		if (somefile.at(i) == '.') { return result; }
		result += somefile.at(i);
	}
	return result;
}

#endif // !P0_H

