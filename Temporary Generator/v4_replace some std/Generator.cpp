// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-15

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-12-11

#include "Generator.h"

// Constructor
Generator::Generator(const Tree& parse_tree) : tree(parse_tree), label_count(0), temp_count(0) {}

// Getter
string Generator::get_code() const { return code.str(); }

// Setter
void Generator::set_code(const string& new_code) { code.str(new_code); }

// Member functions

// Create a unique label
string Generator::create_label() {
    ostringstream label;
    label << "L" << label_count++;
    return label.str();
}

// Create a unique temporary variable
string Generator::create_temp() {
    ostringstream temp;
    temp << "T" << temp_count++;
    string temp_str = temp.str();
    allocate_storage(temp_str);  // Track the temporary variable
    
    return temp.str();
}

// Track the storage of a variable
void Generator::allocate_storage(const string& variable_name) {
    // Track declared variable
    if (!variable_name.empty()) {
        declared_variables.push_back(variable_name);
    }
}

// Traverse the parse tree
void Generator::traverse(const Node& node) {
    const string& data = node.get_data();

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
        const vector<Node>& children = node.get_children();
        for (size_t i = 0; i < children.size(); ++i) {
            traverse(children.at(i));
        }
    }
}

// Generate the code
void Generator::generate() {
        traverse(tree.get_root());
        code << "STOP\n";

        // Add storage for global variables
        for (size_t i = 0; i < declared_variables.size(); ++i) {
            code << declared_variables.at(i) << " 0\n";
        }

}

// Handle the <program> node
void Generator::handle_program(const Node& node) {
    const vector<Node>& children = node.get_children();
    if (children.size() > 1) { traverse(children.at(1));} // Traverse <vars>
    if (children.size() > 2) { traverse(children.at(2));} // Traverse <block> 
}

// Handle the <vars> node
void Generator::handle_var_list(const Node& node) {
    const vector<Node>& children = node.get_children();

    if (children.size() == 4 && children.at(3).get_data() == ";") {
        // Base case: The last (identifier , integer ;)
        const string& var_name = children.at(0).get_data();  // Extract the variable name
        allocate_storage(var_name);  // Add to declared variables

    } else if (children.size() == 4) {
        // Recursive case: Process the first identifier, integer pair
        const string& var_name = children.at(0).get_data();  // Extract the variable name
        allocate_storage(var_name);  // Add to declared variables

        // Recur on the next part of <varList>
        const Node& next_var_list = children.at(3);  // The 4th child is the remaining <varList>
        handle_var_list(next_var_list);
    } else {
        cerr << "Error: Malformed <varList> node.\n";
    }
}

// Handle the <block> node
void Generator::handle_block(const Node& node) {
    const vector<Node>& children = node.get_children();
    
    if (children.size() > 1) { traverse(children.at(1));} // Traverse <vars>
    if (children.size() > 2) { traverse(children.at(2));} // Traverse <stats>
}

// Handle the <print> node
void Generator::handle_print(const Node& node) {
   const vector<Node>& children = node.get_children();

    if (!children.empty()) {
        const Node& exp_node = children.at(1);
        
        // Traverse to evaluate the expression node
        traverse(exp_node);
        const string& value = get_terminal_value(exp_node);

        // Check if the value is an identifier or a literal
        if (!value.empty() && isalpha(value.at(0))) {
            code << "WRITE " << value << "\n"; // Write the value of the variable
        } else {
            string temp = create_temp();  // Create a temporary variable
            code << "STORE " << temp << "\n";  // Store the literal value in the temporary
            code << "WRITE " << temp << "\n";  // Write the value from the temporary
        }
    }
}

// Handle the <stat> node
void Generator::handle_stat(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Ensure there is exactly one child in the <stat> node
    if (children.size() != 1) {
        cerr << "Error: <stat> node has an invalid structure." << endl;
        return;
    }

    const string& child_label = children.at(0).get_data();

    // Determine which type of <stat> to process based on the child node's label
    if (child_label == "<read>") {
        handle_read(children.at(0));
    } else if (child_label == "<print>") {
        handle_print(children.at(0));
    } else if (child_label == "<block>") {
        handle_block(children.at(0));
    } else if (child_label == "<cond>") {
        handle_cond(children.at(0));
    } else if (child_label == "<iter>") {
        handle_iter(children.at(0));
    } else if (child_label == "<assign>") {
        handle_assign(children.at(0));
    } else {
        cerr << "Error: Unknown statement type in <stat> node: " << child_label << endl;
    }
}

// Handle the <stats> node
void Generator::handle_stats(const Node& node) {
    const vector<Node>& children = node.get_children();
    for (size_t i = 0; i < children.size(); ++i) {
        traverse(children.at(i));  // Traverse each <stat>
    }
}

// Handle the <mStat> node
void Generator::handle_m_stat(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Base case: empty production
    if (children.empty()) {
        cout << "Empty <mStat> node. Nothing to process.\n";
        return;
    }

    // Recursive case: <stat> <mStat>
    if (children.size() == 2) {
        traverse(children.at(0));  // Process <stat>
        handle_m_stat(children.at(1));  // Recur on <mStat>
    } else {
        cerr << "Error: Unexpected structure in <mStat> node.\n";
    }
}

// Handle the <vars> node
void Generator::handle_vars(const Node& node) {
    const vector<Node>& children = node.get_children();
    if (children.size() > 1 && children.at(0).get_data() == "var") {
        traverse(children.at(1));  // Traverse <varList>
    } else {
        cout << "No variables to declare\n";
    }
}

// Handle the <iter> node
void Generator::handle_iter(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Generate labels for the loop
    string loop_start = create_label();
    string loop_end = create_label();

    // Start of the loop
    code << loop_start << ": ";

    // Evaluate the left-hand <exp> 
    traverse(children.at(2));
    string left_temp = create_temp();
    code << "STORE " << left_temp << "\n";

    // Evaluate the right-hand <exp>
    traverse(children.at(4));
    string right_temp = create_temp();
    code << "STORE " << right_temp << "\n";

    // Generate code for the condition (left - right)
    code << "LOAD " << left_temp << "\n";
    code << "SUB " << right_temp << "\n";

    // Handle the relational operator
    const Node& relational_node = children.at(3);

    if (relational_node.get_children().empty()) {
        handle_relational(relational_node);  // Directly process if not nested
    } else {
        const string& rel_op = relational_node.get_children().at(0).get_data();
        handle_relational(relational_node.get_children().at(0));
    }

    code << loop_end << "\n";

    // Traverse the <stat>
    traverse(children.at(6));

    // Jump back to the start of the loop
    code << "BR " << loop_start << "\n";

    // Add the end of the loop
    code << loop_end << ": NOOP\n";
}

// Handle the <read> node
void Generator::handle_read(const Node& node) {
    const string& var_name = node.get_children().at(1).get_data();
    code << "READ " << var_name << "\n"; 
}

// Handle the <cond> node
void Generator::handle_cond(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Traverse and evaluate the expressions
    traverse(children.at(2));  // Left-hand <exp>
    string left = create_temp();
    code << "STORE " << left << "\n";

    traverse(children.at(4));  // Right-hand <exp>
    string right = create_temp();
    code << "STORE " << right << "\n";

    code << "LOAD " << left << "\n";
    code << "SUB " << right << "\n";

    // Generate labels for branching
    string label = create_label();

    // Handle the relational operator
    const Node& relational_node = children.at(3);
    

    if (relational_node.get_children().empty()) {
        handle_relational(relational_node);  // Directly process if not nested
    } else {
        const string& rel_op = relational_node.get_children().at(0).get_data();
    
        handle_relational(relational_node.get_children().at(0));
    }

    code << label << "\n";

    // Process the statement inside the condition
    traverse(children.at(6)); // Traverse the <stat>

    // Add label to the code
    code << label << ": NOOP\n";
}

// Handle the <relational> node
void Generator::handle_relational(const Node& node) {

    const string& rel_op = node.get_data();  // Get the relational operator
    if (rel_op == ".ge.") {
        code << "BRNEG ";
    } else if (rel_op == ".le.") {
        code << "BRPOS ";
    } else if (rel_op == ".gt.") {
        code << "BRZNEG ";
    } else if (rel_op == ".lt.") {
        code << "BRZPOS ";
    } else if (rel_op == "**") {
        code << "BRZERO ";  // Branch if equal
    } else if (rel_op == "~") {
        // Emulate "branch if not equal" using BRZERO and BR
        string false_label = create_label();  // Create a label for the false branch
        code << "BRZERO " << false_label << "\n";  // Skip if equal
        code << "BR ";                             // Unconditional branch (to true branch)
        code << false_label << ": NOOP\n";         // Define the false label
    }
}

// Handle the <exp> node
void Generator::handle_exp(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Ensure there are children before proceeding
    if (children.empty()) {
        cerr << "Error: <exp> node has no children!" << endl;
        return;
    }

    if (children.size() > 3) {
        // Process complex expressions with more than three children
        const string& op = children.at(1).get_data();

        if (op == "+" || op == "-") {
            // Create a sub-expression node for the right-hand side
            Node sub_exp;
            sub_exp.set_data("<exp>");
            for (size_t i = 2; i < children.size(); i++) {
                sub_exp.add_child(children.at(i));
            }

            handle_exp(sub_exp); // Process the sub-expression

            // Store the result of the sub-expression in a temporary
            string right_temp = create_temp();
            code << "STORE " << right_temp << "\n";

            // Process the left-hand side
            handle_m(children.at(0));

            // Generate the operation
            if (op == "+") {
                code << "ADD " << right_temp << "\n";
            } else if (op == "-") {
                code << "SUB " << right_temp << "\n";
            }
        } else {
            // Treat the entire node as an expression
            for (size_t i = 0; i < children.size(); i++) {
                handle_m(children.at(i));
                if (i < children.size() - 1) {
                    const string& sub_op = children[i + 1].get_data();
                    string temp = create_temp();
                    code << "STORE " << temp << "\n";
                    if (sub_op == "%") {
                        code << "MULT " << temp << "\n";
                    } else if (sub_op == "/") {
                        code << "DIV " << temp << "\n";
                    }
                }
            }
        }
    } else if (children.size() == 3) {
        // Process binary operations
        handle_m(children.at(2)); // Right-hand operand
        string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_m(children.at(0)); // Left-hand operand

        const string& operator_token = children.at(1).get_data();
        if (operator_token == "+") {
            code << "ADD " << right_temp << "\n";
        } else if (operator_token == "-") {
            code << "SUB " << right_temp << "\n";
        }
    } else if (children.size() == 1) {
        // Process a single operand
        handle_m(children.at(0));
    } else {
        cerr << "Error: Invalid <exp> node structure.\n";
    }
}

// Handle the <M> node
void Generator::handle_m(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Ensure the node has children
    if (children.empty()) {
        cerr << "Error: <M> node has no children!" << endl;
        return;
    }

    if (children.size() == 1) {
        // Base case: Single <N> node
        handle_n(children.at(0));
    } else if (children.size() == 3 && children.at(1).get_data() == "%") {
        // Case: Simple <M> -> <N> % <M>
        handle_m(children.at(2)); // Process the right-hand side
        string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_n(children.at(0)); // Process the left-hand side
        code << "MULT " << right_temp << "\n";
    } else if (children.size() > 3 && children.at(1).get_data() == "%") {
        // Case: Complex <M> with multiple % operators
        Node sub_m;
        sub_m.set_data("<M>");
        for (size_t i = 2; i < children.size(); i++) {
            sub_m.add_child(children.at(i));
        }

        handle_m(sub_m); // Process the remaining part of <M>

        string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_n(children.at(0)); // Process the left-most child
        code << "MULT " << right_temp << "\n";
    } else {
        // Case: Default (delegate to <N>)
        handle_n(node);
    }
}

// Handle the <N> node
void Generator::handle_n(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Ensure the node has children to process
    if (children.empty()) {
        cerr << "Error: <N> node has no children!" << endl;
        return;
    }

    if (children.size() == 1) {
        // Base case: Single <R> node
        handle_r(children.at(0));
    } else if (children.size() == 2 && children.at(0).get_data() == "-") {
        // Unary negation: -<N>
        handle_n(children.at(1)); // Process the operand
        string temp = create_temp();
        code << "STORE " << temp << "\n"; // Store the result of the operand
        code << "LOAD 0\n";               // Load 0 into the accumulator
        code << "SUB " << temp << "\n";   // Subtract the operand from 0 to negate
    } else if (children.size() == 3 && children.at(1).get_data() == "/") {
        // Binary division: <R> / <N>
        handle_r(children.at(0)); // Process the left operand
        string left_temp = create_temp();
        code << "STORE " << left_temp << "\n";

        // Traverse the right operand properly
        traverse(children.at(2)); // Process the right operand
        string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        code << "LOAD " << left_temp << "\n"; // Load the left operand back into the accumulator
        code << "DIV " << right_temp << "\n"; // Divide left by right
    } else if (children.size() > 3 && children.at(1).get_data() == "/") {
        // Complex division case

        // Handle the sub-expression for the right side
        Node sub_n;
        sub_n.set_data("<N>");
        for (size_t i = 2; i < children.size(); ++i) {
            sub_n.add_child(children.at(i));
        }

        traverse(sub_n); // Process the remaining part of <N>
        string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_r(children.at(0)); // Process the left-most child
        string left_temp = create_temp();
        code << "STORE " << left_temp << "\n";

        code << "LOAD " << left_temp << "\n";
        code << "DIV " << right_temp << "\n";
    } else {
        // Default error case for unexpected structures
        cerr << "Error: Unexpected structure in <N> node." << endl;
    }

}

// Handle the <R> node
void Generator::handle_r(const Node& node) {
    const vector<Node>& children = node.get_children();

    // Ensure the node has children to process
    if (children.empty()) {
        cerr << "Error: <R> node has no children!" << endl;
        return;
    }

    // Case 1: Single terminal node (variable or literal)
    if (children.size() == 1) {
        const string& data = children.at(0).get_data();
        if (data.empty()) {
            cerr << "Error: Terminal node in <R> is empty!" << endl;
            return;
        }

        // Determine if the terminal node is a variable or literal
        if (isalpha(data.at(0))) {
            // If the first character is alphabetic, it's a variable
            code << "LOAD " << data << "\n";
        } else {
            // Otherwise, it's treated as a literal
            code << "LOAD " << data << "\n";
        }
    }

    // Case 2: Parenthesized expression
    else if (children.size() == 3 && children.at(0).get_data() == "(" && children.at(2).get_data() == ")") {
        handle_exp(children.at(1)); // Process the expression inside parentheses
    }

    // Case 3: Complex expression in <R>
    else if (children.size() > 1) {

        // Create a new <exp> node for the inner expression
        Node exp;
        exp.set_data("<exp>");
        for (size_t i = 0; i < children.size(); ++i) {
            exp.add_child(children.at(i));
        }
        handle_exp(exp); // Process the new expression node
    }

    // Default error case for unexpected structures
    else {
        cerr << "Error: Unexpected structure in <R> node." << endl;
    }
}

// Handle the <assign> node
void Generator::handle_assign(const Node& node) {
    const vector<Node>& children = node.get_children();
    const string& var_name = children.at(1).get_data();  

    traverse(children.at(2));  // Traverse the expression to evaluate
    code << "STORE " << var_name << "\n";

    cout << "Handled <assign> node.\n";
}

// Get the value of a terminal node
string Generator::get_terminal_value(const Node& node) {
    const vector<Node>& children = node.get_children();

    // If there are no children, this is a terminal node
    if (children.empty()) {
        return node.get_data();  // Return the actual data from the terminal node
    }

    // If there are children, keep traversing to find the terminal node
    return get_terminal_value(children.at(0));
}





