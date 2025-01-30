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

    const std::vector<Node>& children = node.get_children();

    // Ensure there is exactly one child in the <stat> node
    if (children.size() != 1) {
        std::cerr << "Error: <stat> node has an invalid structure." << std::endl;
        return;
    }

    const std::string& child_label = children[0].get_data();

    // Determine which type of <stat> to process based on the child node's label
    if (child_label == "<read>") {
        handle_read(children[0]);
    } else if (child_label == "<print>") {
        handle_print(children[0]);
    } else if (child_label == "<block>") {
        handle_block(children[0]);
    } else if (child_label == "<cond>") {
        handle_cond(children[0]);
    } else if (child_label == "<iter>") {
        handle_iter(children[0]);
    } else if (child_label == "<assign>") {
        handle_assign(children[0]);
    } else {
        std::cerr << "Error: Unknown statement type in <stat> node: " << child_label << std::endl;
    }

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

    // Base case: empty production
    if (children.empty()) {
        std::cout << "Empty <mStat> node. Nothing to process.\n";
        return;
    }

    // Recursive case: <stat> <mStat>
    if (children.size() == 2) {
        traverse(children[0]);  // Process <stat>
        handle_m_stat(children[1]);  // Recur on <mStat>
    } else {
        std::cerr << "Error: Unexpected structure in <mStat> node.\n";
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

    // Generate labels for the loop
    std::string loop_start = create_label();
    std::string loop_end = create_label();

    // Start of the loop
    code << loop_start << ": ";

    // Evaluate the left-hand <exp> (children[3])
    traverse(children[2]);
    std::string left_temp = create_temp();
    code << "STORE " << left_temp << "\n";

    // Evaluate the right-hand <exp> (children[5])
    traverse(children[4]);
    std::string right_temp = create_temp();
    code << "STORE " << right_temp << "\n";

    // Generate code for the condition (left - right)
    code << "LOAD " << left_temp << "\n";
    code << "SUB " << right_temp << "\n";

    // Handle the relational operator (children[4])
    const Node& relational_node = children.at(3);

    if (relational_node.get_children().empty()) {
        handle_relational(relational_node);  // Directly process if not nested
    } else {
        const string& rel_op = relational_node.get_children().at(0).get_data();
        handle_relational(relational_node.get_children().at(0));
    }

    code << loop_end << "\n";

    // Traverse the <stat> (children[7]), which is the body of the loop
    traverse(children[6]);

    // Jump back to the start of the loop
    code << "BR " << loop_start << "\n";

    // Add the end of the loop
    code << loop_end << ": NOOP\n";

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

    // Traverse and evaluate the expressions
    traverse(children[2]);  // Left-hand <exp>
    std::string left = create_temp();
    code << "STORE " << left << "\n";

    traverse(children[4]);  // Right-hand <exp>
    std::string right = create_temp();
    code << "STORE " << right << "\n";

    code << "LOAD " << left << "\n";
    code << "SUB " << right << "\n";

    // Generate labels for branching
    std::string label = create_label();

    // Handle the relational operator
    const Node& relational_node = children[3];
    std::cout << "Relational operator node data: " << relational_node.get_data() << "\n";

    if (relational_node.get_children().empty()) {
        handle_relational(relational_node);  // Directly process if not nested
    } else {
        const std::string& rel_op = relational_node.get_children()[0].get_data();
        std::cout << "Extracted relational operator: " << rel_op << "\n";
        handle_relational(relational_node.get_children()[0]);
    }

    code << label << "\n";

    // Process the statement inside the condition
    traverse(children[6]);  // Traverse <stat> (e.g., WRITE x1)

    // Add label to the code
    code << label << ": NOOP\n";

    std::cout << "Handled <cond> node.\n";
}

void Generator::handle_relational(const Node& node) {
    std::cout << "Handling <relational> node..." << std::endl;

    const std::string& rel_op = node.get_data();  // Get the relational operator
    if (rel_op == ".ge.") {
        code << "BRNEG ";
    } else if (rel_op == ".le.") {
        code << "BRPOS ";
    } else if (rel_op == ".gt.") {
        code << "BRZNEG ";
    } else if (rel_op == ".lt.") {
        code << "BRZPOS ";
    } else if (rel_op == "~") {
        code << "BRZERO ";  // Branch if equal
    } else if (rel_op == "**") {
        // Emulate "branch if not equal" using BRZERO and BR
        std::string false_label = create_label();  // Create a label for the false branch
        code << "BRZERO " << false_label << "\n";  // Skip if equal
        code << "BR ";                             // Unconditional branch (to true branch)
        code << false_label << ": NOOP\n";         // Define the false label
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
        // Process complex expressions with more than three children
        const std::string& op = children[1].get_data();

        if (op == "+" || op == "-") {
            // Create a sub-expression node for the right-hand side
            Node sub_exp;
            sub_exp.set_data("<exp>");
            for (size_t i = 2; i < children.size(); i++) {
                sub_exp.add_child(children[i]);
            }

            handle_exp(sub_exp); // Process the sub-expression

            // Store the result of the sub-expression in a temporary
            std::string right_temp = create_temp();
            code << "STORE " << right_temp << "\n";

            // Process the left-hand side
            handle_m(children[0]);

            // Generate the operation
            if (op == "+") {
                code << "ADD " << right_temp << "\n";
            } else if (op == "-") {
                code << "SUB " << right_temp << "\n";
            }
        } else {
            // Treat the entire node as an expression
            for (size_t i = 0; i < children.size(); i++) {
                handle_m(children[i]);
                if (i < children.size() - 1) {
                    const std::string& sub_op = children[i + 1].get_data();
                    std::string temp = create_temp();
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
        // Process binary operations (e.g., x + y)
        handle_m(children[2]); // Right-hand operand
        std::string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_m(children[0]); // Left-hand operand

        const std::string& operator_token = children[1].get_data();
        if (operator_token == "+") {
            code << "ADD " << right_temp << "\n";
        } else if (operator_token == "-") {
            code << "SUB " << right_temp << "\n";
        }
    } else if (children.size() == 1) {
        // Process a single operand
        handle_m(children[0]);
    } else {
        std::cerr << "Error: Invalid <exp> node structure.\n";
    }

    std::cout << "Handled <exp> node.\n";
}

void Generator::handle_m(const Node& node) {
    const std::vector<Node>& children = node.get_children();

    // Ensure the node has children
    if (children.empty()) {
        std::cerr << "Error: <M> node has no children!" << std::endl;
        return;
    }

    if (children.size() == 1) {
        // Base case: Single <N> node
        handle_n(children[0]);
    } else if (children.size() == 3 && children[1].get_data() == "%") {
        // Case: Simple <M> -> <N> % <M>
        handle_m(children[2]); // Process the right-hand side
        std::string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_n(children[0]); // Process the left-hand side
        code << "MULT " << right_temp << "\n";
    } else if (children.size() > 3 && children[1].get_data() == "%") {
        // Case: Complex <M> with multiple % operators
        Node sub_m;
        sub_m.set_data("<M>");
        for (size_t i = 2; i < children.size(); i++) {
            sub_m.add_child(children[i]);
        }

        handle_m(sub_m); // Process the remaining part of <M>

        std::string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_n(children[0]); // Process the left-most child
        code << "MULT " << right_temp << "\n";
    } else {
        // Case: Default (delegate to <N>)
        handle_n(node);
    }
}

void Generator::handle_n(const Node& node) {
    std::cout << "Handling <N> node..." << std::endl;

    const std::vector<Node>& children = node.get_children();

    // Ensure the node has children to process
    if (children.empty()) {
        std::cerr << "Error: <N> node has no children!" << std::endl;
        return;
    }

    if (children.size() == 1) {
        // Base case: Single <R> node
        handle_r(children[0]);
    } else if (children.size() == 2 && children[0].get_data() == "-") {
        // Unary negation: -<N>
        handle_n(children[1]); // Process the operand
        std::string temp = create_temp();
        code << "STORE " << temp << "\n"; // Store the result of the operand
        code << "LOAD 0\n";               // Load 0 into the accumulator
        code << "SUB " << temp << "\n";   // Subtract the operand from 0 to negate
    } else if (children.size() == 3 && children[1].get_data() == "/") {
        // Binary division: <R> / <N>
        handle_r(children[0]); // Process the left operand
        std::string left_temp = create_temp();
        code << "STORE " << left_temp << "\n";

        // Traverse the right operand properly
        traverse(children[2]); // Process the right operand
        std::string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        code << "LOAD " << left_temp << "\n"; // Load the left operand back into the accumulator
        code << "DIV " << right_temp << "\n"; // Divide left by right
    } else if (children.size() > 3 && children[1].get_data() == "/") {
        // Complex division case
        std::cout << "Complex division case\n";

        // Handle the sub-expression for the right side
        Node sub_n;
        sub_n.set_data("<N>");
        for (size_t i = 2; i < children.size(); ++i) {
            sub_n.add_child(children[i]);
        }

        traverse(sub_n); // Process the remaining part of <N>
        std::string right_temp = create_temp();
        code << "STORE " << right_temp << "\n";

        handle_r(children[0]); // Process the left-most child
        std::string left_temp = create_temp();
        code << "STORE " << left_temp << "\n";

        code << "LOAD " << left_temp << "\n";
        code << "DIV " << right_temp << "\n";
    } else {
        // Default error case for unexpected structures
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
        const std::string& data = children[0].get_data();
        if (data.empty()) {
            std::cerr << "Error: Terminal node in <R> is empty!" << std::endl;
            return;
        }

        // Determine if the terminal node is a variable or literal
        if (std::isalpha(data[0])) {
            // If the first character is alphabetic, it's a variable
            code << "LOAD " << data << "\n";
        } else {
            // Otherwise, it's treated as a literal
            code << "LOAD " << data << "\n";
        }
    }

    // Case 2: Parenthesized expression (e.g., `( <exp> )`)
    else if (children.size() == 3 && children[0].get_data() == "(" && children[2].get_data() == ")") {
        std::cout << "Processing parenthesized expression in <R>...\n";
        handle_exp(children[1]); // Process the expression inside parentheses
    }

    // Case 3: Complex expression in <R>
    else if (children.size() > 1) {
        std::cout << "Processing complex expression in <R>...\n";

        // Create a new <exp> node for the inner expression
        Node exp;
        exp.set_data("<exp>");
        for (size_t i = 0; i < children.size(); ++i) {
            exp.add_child(children[i]);
        }

        handle_exp(exp); // Process the new expression node
    }

    // Default error case for unexpected structures
    else {
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





