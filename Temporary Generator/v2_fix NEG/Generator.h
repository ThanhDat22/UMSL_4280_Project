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
    const Tree& tree;
    ostringstream code;

    size_t label_count;
    size_t temp_count;

    vector<string> declared_variables;

    // Member functions
    string create_label();
    string create_temp();

    void allocate_storage(const string&);
    string get_terminal_value(const Node& node);

    void traverse(const Node& node);
    void handle_program(const Node& node);
    void handle_vars(const Node& node);
    void handle_var_list(const Node& node);
    void handle_stats(const Node& node);
    void handle_m_stat(const Node& node);
    void handle_assign(const Node& node);
    void handle_block(const Node& node);
    void handle_stat(const Node& node);
    void handle_read(const Node& node);
    void handle_print(const Node& node);
    void handle_cond(const Node& node);
    void handle_iter(const Node& node);
    void handle_relational(const Node& node);
    void handle_exp(const Node& node);
    void handle_m(const Node& node);
    void handle_n(const Node& node);
    void handle_r(const Node& node);
};






#endif 