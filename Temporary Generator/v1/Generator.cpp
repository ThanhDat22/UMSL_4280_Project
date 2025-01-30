#include "Generator.h"

// Constructor
Generator::Generator(const Tree& parse_tree) : tree(parse_tree), label_count(0), temp_count(0) {}

// Getter
string Generator::get_code() const { return code.str(); }

// Setter
void Generator::set_code(const string& new_code) { code.str(new_code); }

// Member functions
string Generator::create_label() {
    ostringstream label;
    label << "L" << label_count++;
    return label.str();
}

string Generator::create_temp() {
    ostringstream temp;
    temp << "T" << temp_count++;
    string temp_str = temp.str();
    allocate_storage(temp_str);  // Track the temporary variable
    
    return temp.str();
}

void Generator::allocate_storage(const std::string& variable_name) {
    // Track declared variable
    if (!variable_name.empty()) {
        declared_variables.push_back(variable_name);
        std::cout << "Tracking variable: " << variable_name << std::endl;
    }
}

void Generator::traverse(const Node& node) {
    const std::string& data = node.get_data();
    std::cout << "Traversing node: " << data << std::endl;

    if (data == "<program>") { handle_program(node); }
    else if (data == "<vars>") { handle_vars(node); }
    else if (data == "<varList>") { handle_var_list(node); }
    else if (data == "<block>") { handle_block(node); }
    else if (data == "<stats>") { handle_stats(node); }
    else if (data == "<mStat>") { handle_m_stat(node); }
    else if (data == "<stat>") { handle_stat(node); }
    else if (data == "<read>") { handle_read(node); }
    else if (data == "<print>") { handle_print(node); }
    else if (data == "<assign>") { handle_assign(node); }
    else if (data == "<cond>") { handle_cond(node); }
    else if (data == "<iter>") { handle_iter(node); }
    else if (data == "<relational>") { handle_relational(node); }
    else if (data == "<exp>") { handle_exp(node); }
    else if (data == "<M>") { handle_m(node); }
    else if (data == "<N>") { handle_n(node); }
    else if (data == "<R>") { handle_r(node); }
    else {
        const std::vector<Node>& children = node.get_children();
        for (size_t i = 0; i < children.size(); ++i) {
            traverse(children.at(i));
        }
    }
}

void Generator::generate() {
    std::cout << "Generating code..." << std::endl;
        traverse(tree.get_root());
        code << "STOP\n";

        // Add storage for global variables
        for (size_t i = 0; i < declared_variables.size(); ++i) {
            code << declared_variables[i] << " 0\n";
        }

}

void Generator::handle_program(const Node& node) {
    std::cout << "Handling <program> node..." << std::endl;
    const std::vector<Node>& children = node.get_children();
    if (children.size() > 1) { traverse(children[1]);} // Traverse <vars>
    if (children.size() > 2) { traverse(children[2]);} // Traverse <block> 
    
    std::cout << "Handled <program> node.\n";
}

void Generator::handle_var_list(const Node& node) {
    std::cout << "Handling <varList> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();

    if (children.size() == 4 && children[3].get_data() == ";") {
        // Base case: The last variable declaration (identifier , integer ;)
        const std::string& var_name = children[0].get_data();  // Extract the variable name
        allocate_storage(var_name);  // Add to declared variables
        std::cout << "Processed final variable: " << var_name << std::endl;
    } else if (children.size() == 4) {
        // Recursive case: Process the first identifier, integer pair and continue
        const std::string& var_name = children[0].get_data();  // Extract the variable name
        allocate_storage(var_name);  // Add to declared variables
        std::cout << "Processed variable: " << var_name << std::endl;

        // Recur on the next part of <varList>
        const Node& next_var_list = children[3];  // The 4th child is the remaining <varList>
        handle_var_list(next_var_list);
    } else {
        std::cerr << "Error: Malformed <varList> node.\n";
    }

    std::cout << "Handled <varList> node.\n";
}

void Generator::handle_block(const Node& node) {
    std::cout << "Handling <block> node..." << std::endl;
    const std::vector<Node>& children = node.get_children();
    
    if (children.size() > 1) { traverse(children[1]);} // Traverse <vars>
    if (children.size() > 2) { traverse(children[2]);} // Traverse <stats>
    
    std::cout << "Handled <block> node.\n";
}

void Generator::handle_print(const Node& node) {
    std::cout << "Handling <print> node..." << std::endl;
    const std::vector<Node>& children = node.get_children();

    if (!children.empty()) {
        const Node& exp_node = children[1];
        
        // Traverse to evaluate the expression node, which should load value into the accumulator
        traverse(exp_node);

        // Now we need to find the actual value (literal or identifier)
        const std::string& value = get_terminal_value(exp_node);

        // Check if the value is an identifier or a literal
        if (!value.empty() && isalpha(value[0])) {
            // If the value is an identifier (e.g., variable name like 'x1')
            // Directly write the value of the variable
            code << "WRITE " << value << "\n";
        } else {
            // If it's a literal value (e.g., an integer like 1)
            // Assume the value has already been loaded into the accumulator by traverse()
            std::string temp = create_temp();  // Create a temporary variable
            code << "STORE " << temp << "\n";  // Store the literal value in the temporary
            code << "WRITE " << temp << "\n";  // Write the value from the temporary
        }
    }

    std::cout << "Handled <print> node.\n";
    std::cout << "Current generated code:\n" << code.str() << std::endl;
}

void Generator::handle_stat(const Node& node) {
    std::cout << "Handling <stat> node..." << std::endl;
    traverse(node.get_children().at(0));
    std::cout << "Handled <stat> node.\n";
}

void Generator::handle_stats(const Node& node) {
    std::cout << "Handling <stats> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();
    for (size_t i = 0; i < children.size(); ++i) {
        traverse(children[i]);  // Traverse each <stat>
    }

    std::cout << "Handled <stats> node.\n";
}

void Generator::handle_m_stat(const Node& node) {
    std::cout << "Handling <mStat> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();
    for (size_t i = 0; i < children.size(); ++i) {
        traverse(children[i]);  // Traverse each <stat>
    }

    std::cout << "Handled <mStat> node.\n";
}

void Generator::handle_vars(const Node& node) {
    std::cout << "Handling <vars> node..." << std::endl;
    const std::vector<Node>& children = node.get_children();
    if (children.size() > 1 && children[0].get_data() == "var") {
        traverse(children[1]);  // Traverse <varList>
    } else {
        std::cout << "No variables to declare\n";
    }
    std::cout << "Handled <vars> node.\n";
}

void Generator::handle_iter(const Node& node) {
    std::cout << "Handling <iter> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();
    std::string start_label = create_label();
    std::string end_label = create_label();

    code << start_label << ": NOOP\n";

    traverse(children[2]);  // Traverse left <exp>
    std::string temp = create_temp();
    code << "STORE " << temp << "\n";

    traverse(children[4]);  // Traverse right <exp>
    code << "SUB " << temp << "\n";

    handle_relational(children[3]);
    code << end_label << "\n";

    traverse(children[6]);  // Traverse <stat> inside the loop
    code << "BR " << start_label << "\n";

    code << end_label << ": NOOP\n";
    std::cout << "Handled <iter> node.\n";
}

void Generator::handle_read(const Node& node) {
    std::cout << "Handling <read> node..." << std::endl;

    const std::string& var_name = node.get_children()[1].get_data();  // Assuming get_value() returns the variable name
    code << "READ " << var_name << "\n";

    std::cout << "Handled <read> node.\n";
}

void Generator::handle_cond(const Node& node) {
    std::cout << "Handling <cond> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();
    traverse(children[2]);  // Traverse left <exp>
    std::string temp = create_temp();
    code << "STORE " << temp << "\n";

    traverse(children[4]);  // Traverse right <exp>
    code << "SUB " << temp << "\n";

    std::string label = create_label();
    handle_relational(children[3]);
    code << label << "\n";

    traverse(children[6]);  // Traverse <stat>
    code << label << ": NOOP\n";

    std::cout << "Handled <cond> node.\n";
}

void Generator::handle_relational(const Node& node) {
    std::cout << "Handling <relational> node..." << std::endl;

    const std::string& rel_op = node.get_data();  // Get the relational operator
    if (rel_op == ".le.") {
        code << "BRLE ";
    } else if (rel_op == ".ge.") {
        code << "BRGE ";
    } else if (rel_op == ".lt.") {
        code << "BRLT ";
    } else if (rel_op == ".gt.") {
        code << "BRGT ";
    } else if (rel_op == "**") {
        code << "BREQ ";  // Branch if equal
    } else if (rel_op == "~") {
        code << "BRNE ";  // Branch if not equal
    }

    std::cout << "Handled <relational> node.\n";
}

void Generator::handle_exp(const Node& node) {
    std::cout << "Handling <exp> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();

    // Ensure there are children before proceeding
    if (children.empty()) {
        std::cerr << "Error: <exp> node has no children!" << std::endl;
        return;
    }

    if (children.size() > 3) {
        const std::string& op = children[1].get_data();

        if ( op == "+" || op == "-") {
            Node exp;
            exp.set_data("<exp>");
            for(size_t i = 2; i < children.size(); i++) {
                exp.add_child(children[i]);
            }
            handle_exp(exp);

            // Store the result of the right sub-expression in a temporary
            std::string right_temp = create_temp();
            code << "STORE " << right_temp << "\n";

            handle_m(children[0]);

            if (op == "+") {
                code << "ADD " << right_temp << "\n";
            } else if (op == "-") {
                code << "SUB " << right_temp << "\n";
            }
        } else {
            // If the operator is not "+" or "-", treat the entire expression as an `m`
            handle_m(node);
        }
    }
    // Case: Simple binary operation
    else if (children.size() == 3) {
        // Process the right-hand operand
        handle_m(children[2]);

        // Store the result of the right operand in a temporary
        std::string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        // Process the left-hand operand
        handle_m(children[0]);

        // Generate the appropriate operation
        const std::string& operator_token = children[1].get_data();
        if (operator_token == "+") {
            code << "ADD " << right_temp << "\n";
        } else if (operator_token == "-") {
            code << "SUB " << right_temp << "\n";
        }
    }
    // Case: Single operand
    else if (children.size() == 1) {
        handle_m(children[0]);
    }
    // Error: Invalid structure
    else {
        std::cerr << "Error: Invalid <exp> node structure.\n";
    }

    std::cout << "Handled <exp> node.\n";
        
}

void Generator::handle_m(const Node& node) {
    const std::vector<Node>& children = node.get_children();

    if (children.empty()) {
        std::cerr << "Error: <M> node has no children!" << std::endl;
        return;
    }

    if (children.size() == 1) {
        // Single <N> case
        handle_n(children[0]);
    } else if (children.size() == 3 && children[1].get_data() == "%") {
        handle_m(children[2]); // Process right-hand side
        std::string temp = create_temp();
        code << "STORE " << temp << "\n";

        handle_n(children[0]); // Process left-hand side
        code << "MUL " << temp << "\n";
    } else if (children.size() > 3 && children[1].get_data() == "%") {
        Node sub_m;
        sub_m.set_data("<M>");
        for (size_t i = 2; i < children.size(); i++) {
            sub_m.add_child(children[i]);
        }

        handle_m(sub_m); // Process remaining part of <M>

        std::string temp = create_temp();
        code << "STORE " << temp << "\n";

        handle_n(children[0]); // Process left-most child
        code << "MUL " << temp << "\n";
    } else {
        // Default case: delegate to <N>
        handle_n(node);
    }

}

void Generator::handle_n(const Node& node) {
    std::cout << "Handling <N> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();

    if (children.empty()) {
        std::cerr << "Error: <N> node has no children!" << std::endl;
        return;
    }

    if (children.size() == 1) {
        handle_r(children[0]); // Single <R> case
    } else if (children.size() == 2 && children[0].get_data() == "-") {
        // Unary negation
        handle_n(children[1]);
        code << "NEG\n";
    } else if (children.size() == 3 && children[1].get_data() == "/") {
        handle_r(children[0]); // Evaluate left operand
        std::string temp = create_temp();
        code << "STORE " << temp << "\n";

        handle_n(children[2]); // Evaluate right operand
        code << "DIV " << temp << "\n";
    } else {
        std::cerr << "Error: Unexpected structure in <N> node." << std::endl;
    }

    std::cout << "Handled <N> node.\n";
}

void Generator::handle_r(const Node& node) {
    std::cout << "Handling <R> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();

    // Ensure the node has children to process
    if (children.empty()) {
        std::cerr << "Error: <R> node has no children!" << std::endl;
        return;
    }

    // Case 1: Single terminal node (variable or literal)
    if (children.size() == 1) {
        const std::string& data = children.at(0).get_data();
        if (data.empty()) {
            std::cerr << "Error: Terminal node in <R> is empty!" << std::endl;
            return;
        }

        // Generate the LOAD instruction
        code << "LOAD " << data << "\n";

    // Case 2: Parenthesized expression
    } else if (children.size() == 3 && children[0].get_data() == "(") {
        std::cout << "Processing parenthesized expression in <R>...\n";
        handle_exp(children[1]); // Process the expression inside parentheses

    // Case 3: Complex expression
    } else if (children.size() > 1) {
        std::cout << "Processing complex expression in <R>...\n";

        // Create a new <exp> node to represent the expression
        Node exp;
        exp.set_data("<exp>");
        for (size_t i = 1; i < children.size() - 1; ++i) {
            exp.add_child(children[i]);
        }

        handle_exp(exp); // Process the new expression node
    } else {
        std::cerr << "Error: Unexpected structure in <R> node." << std::endl;
    }

    std::cout << "Handled <R> node.\n";
}

void Generator::handle_assign(const Node& node) {
    std::cout << "Handling <assign> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();
    const std::string& var_name = children[1].get_data();  

    traverse(children[2]);  // Traverse the expression to evaluate
    code << "STORE " << var_name << "\n";

    std::cout << "Handled <assign> node.\n";
}

string Generator::get_terminal_value(const Node& node) {
    const std::vector<Node>& children = node.get_children();

    // If there are no children, this is a terminal node
    if (children.empty()) {
        return node.get_data();  // Return the actual data from the terminal node
    }

    // If there are children, keep traversing to find the terminal node
    return get_terminal_value(children[0]);
}

void Generator::simplify_minus(vector<Node>& exp) {
    for (size_t i = 0; i < exp.size() - 2; ++i) {
        if (exp.at(i).get_data() == "-" && exp.at(i + 1).get_data() == "-" && (exp.at(i + 2).get_data() == "-" || exp.at(i + 2).get_data() == "+") ) {
            exp.erase(exp.begin() + i);
            exp.erase(exp.begin() + i);

            std::cout << "Simplified expression: " << exp.at(i).get_data() << std::endl;
            --i; // Adjust the index to account for the removed elements
        }
    }
}

