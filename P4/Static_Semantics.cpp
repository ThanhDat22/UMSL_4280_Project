// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-07

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-07

#include "Static_Semantics.h"

// Constructors
Static_Semantics::Static_Semantics(const Tree& tree, Scanner& scanner) : parse_tree(tree), scanner(scanner) {}
Static_Semantics::Static_Semantics(const Tree& tree, istringstream& iss) : parse_tree(tree), scanner(iss) {}
Static_Semantics::Static_Semantics(const Tree& tree, istream& is) : parse_tree(tree), scanner(is) {}

// Member functions

/** Check the semantics of the parse tree
 * @param node The node to check
 * @param is_declaration True if the node is a declaration, false otherwise
 */
void Static_Semantics::check_semantics(const Node& node) {
    // Check if the node is a declaration or usage
    if (node.get_data() == "<vars>" || node.get_data() == "<varList>") {
        check_declaration(node);
    } else if (node.get_data() == "<read>" || node.get_data() == "<assign>" || node.get_data() == "<print>") {
        check_usage(node);
    }

    // Recursively check all children nodes
    const vector<Node>& children = node.get_children();
    for (size_t i = 0; i < children.size(); i++) {
        check_semantics(children.at(i));
    }
    
}
// Check the semantics of the parse tree wrapper function
void Static_Semantics::check_semantics() {
    check_semantics(parse_tree.get_root());
    symbol_table.check_variable(); // Check whether is there any unused variable after traversing the tree
}

/** Check the semantics of the declaration
 *  @param node The node to check
 */
void Static_Semantics::check_declaration(const Node& node) {
    // Iterate over children to identify variables
    const vector<Node>& children = node.get_children();
    for (size_t i = 0; i < children.size(); ++i) {
        const Node& child = children[i];
        const string& data = child.get_data();
        size_t line_number = child.get_line_number();

        // Check if the child is a variable (adjust as necessary for identifiers)
        if (is_variable(data)) {
            symbol_table.insert(data, line_number);  // Insert each variable
        }
    }
}

/** Check if a string is a variable
 *  @param data The string to check
 *  @return True if the string is a variable, false otherwise
 */
bool Static_Semantics::is_variable(const string& data) {
    bool is_var = !data.empty() && !scanner.is_keyword(data) && isalpha(data[0]);
    return is_var;
}


/** Check the semantics of the usage
 *  @param node The node to check
 */
void Static_Semantics::check_usage(const Node& node) {

    const string& data = node.get_data();
    size_t line_number = node.get_line_number();

    if (is_variable(data)) {
        symbol_table.verify(data, line_number);  
    }

    // Recursively check child nodes if they contain more expressions or variables
    const vector<Node>& children = node.get_children();
    for (size_t i = 0; i < children.size(); i++) {
        check_usage(children.at(i));
    }
}