// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-07

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-07


#ifndef STATIC_SEMANTICS_H
#define STATIC_SEMANTICS_H

#include "Tree.h"
#include "Node.h"
#include "Scanner.h"
#include "Symbol_Table.h"

#include <sstream>

using std::string;
using std::istringstream;

class Static_Semantics {
public:

    // Constructors
    Static_Semantics(const Tree&, Scanner&);
    Static_Semantics(const Tree&, istringstream&);
    Static_Semantics(const Tree&, istream&);

    // Member functions
    void check_semantics(); // Check the semantics of the parse tree

private:

    // Data fields
    const Tree& parse_tree; // Parse tree
    Scanner scanner; 
    Symbol_Table symbol_table; 

    // Member functions
    void check_semantics(const Node&); // Check the semantics of the parse tree recursive function
    void check_declaration(const Node&); // Check the semantics of the declaration
    void check_usage(const Node&); // Check the semantics of the usage
    bool is_variable(const string&); // Check if a string is a variable
                
};

#endif // STATIC_SEMANTICS_H