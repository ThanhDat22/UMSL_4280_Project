// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-15

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-12-10

#ifndef GENERATOR_H
#define GENERATOR_H

#include "Tree.h"
#include "Symbol_Table.h"

#include <string>
#include <sstream>

using std::cerr;
using std::endl;
using std::cout;


class Generator {

public:
    // Constructors
    Generator(const Tree&);

    // Getters
    string get_code() const;

    // Setters
    void set_code(const string&);

    // Member functions
    void generate();

private:
    // Data fields
    const Tree& tree; // The parse tree for code generation
    ostringstream code; // Stores the generated code

    size_t label_count; // Counter for generating unique labels
    size_t temp_count; // Counter for generating unique temporary variables

    vector<string> declared_variables; // Tracks the variables declared in the program

    // Member functions
    string create_label(); // Create a unique label
    string create_temp(); // Create a unique temporary variable

    void allocate_storage(const string&); // Track the storage of a variable
    string get_terminal_value(const Node& node); // Get the value of a terminal node

    void traverse(const Node& node); // Traverse the parse tree
    void handle_program(const Node& node); // Handle the <program> node
    void handle_vars(const Node& node); // Handle the <vars> node
    void handle_var_list(const Node& node); // Handle the <varList> node
    void handle_stats(const Node& node); // Handle the <stats> node
    void handle_m_stat(const Node& node); // Handle the <mStat> node
    void handle_assign(const Node& node); // Handle the <assign> node
    void handle_block(const Node& node); // Handle the <block> node
    void handle_stat(const Node& node); // Handle the <stat> node
    void handle_read(const Node& node); // Handle the <read> node
    void handle_print(const Node& node); // Handle the <print> node
    void handle_cond(const Node& node); // Handle the <cond> node
    void handle_iter(const Node& node); // Handle the <iter> node
    void handle_relational(const Node& node); // Handle the <relational> node
    void handle_exp(const Node& node); // Handle the <exp> node
    void handle_m(const Node& node); // Handle the <M> node
    void handle_n(const Node& node); // Handle the <N> node
    void handle_r(const Node& node); // Handle the <R> node
};






#endif 