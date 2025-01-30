// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-08

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-08

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "Utility.h"

#include <map>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using std::map;
using std::vector;
using std::stack;
using std::string;

class Symbol_Table {

public:
    
    // Constructors
    Symbol_Table(); // Default constructor

    // Member functions
    bool insert(const string&, size_t); // Insert a variable into the symbol table
    bool verify(const string&, size_t); // Verify the existence of a variable in the symbol table
    void check_variable(); // Check the unused variables in the symbol table

    vector<string> get_vars() const; // Get the variables in the symbol table
    bool contains(const string& variable) const; // Check if the symbol table contains a variable
    void enter_scope(); // Enter a new scope
    void exit_scope(); // Exit the current scope
        

private:
    // Data fields
    stack<map<string, bool> > symbol_table; // Store the variables and their status

    // Member functions
    void error_message(const string&); // Reports an error message
    void warning_message(const string&); // Reports a warning message
    //string to_string(const size_t); // Convert a size_t to a string

    

};

#endif // SYMBOL_TABLE_H