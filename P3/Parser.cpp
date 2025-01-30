#include "Parser.h"

// Constructor
Parser::Parser(const string& filename)
    : file_name(filename),
    fin(file_name.c_str()),
    scanner(fin) {
	if (!fin) {
		exit_error("Error: Unable to open the input file.");
	}
	current_token = scanner.get_next_token();
}

Parser::Parser(istringstream& iss)
	: scanner(iss) {
	current_token = scanner.get_next_token();
}

// Member functions

// Create a node with the given data
Node Parser::create_node(const string& data) {
	return Node(data, current_token.line_number);
}

// Fetches the next token
void Parser::next_token() {
	current_token = scanner.get_next_token();
}

/** Matches the current token with the expected token ID and advances to the next token.
    @param expected_id: the expected TokenID to match against the current token
    @throws: exits the program with a syntax error message if the current token does not match expected_id
*/
void Parser::match(TokenID expected_id) {
    if (current_token.id == expected_id) {
        next_token(); // Move to the next token 
    }
    else {
		ostringstream oss;
		oss << current_token.line_number;
        exit_error("Syntax Error was found: Line: " + oss.str() + " " + current_token.instance + ". Expected " + Scanner::get_tkid_name(expected_id));
    }
}

// Parse the input file and return the parse tree
Tree Parser::parse() {
	
    Tree  parse_tree;
    parse_tree.set_root(program());

    if (current_token.id == EOF_TK) { return parse_tree; }
    else {
        exit_error("Syntax error: Unexpected token received, EOF_TK expected.");
        return parse_tree;
    }
}

//<program>  ->     program <vars> <block>
Node Parser::program() {
    Node program_node = create_node("<program>");

    if (current_token.id == KW_TK && current_token.instance == "program") {
        match(KW_TK); //consume 'program'
        program_node.add_child(create_node("program"));
    } else {
        exit_error("Syntax error: Expected 'program' keyword.");
    }

    // Parse <vars> and <block>
    program_node.add_child(vars());
    program_node.add_child(block());

    return program_node;
}

// <vars> -> empty | var <varList>
Node Parser::vars() {
    Node vars_node = create_node("<vars>");

	// Check if the current token is 'var'
    if (current_token.id == KW_TK && current_token.instance == "var") {
        match(KW_TK); //consume 'var' keyword
        vars_node.add_child(create_node("var"));
        vars_node.add_child(var_list()); // Parse the <varList>
    } else {
		return vars_node;
	}

    return vars_node;
}

//<varList> -> identifier, integer; | identifier, integer <varList>
Node Parser::var_list() {
	Node var_list_node = create_node("<varList>");

	// Expect an identifier
	if (current_token.id == IDENT_TK) {
		var_list_node.add_child(create_node(current_token.instance));
		match(IDENT_TK); //consume identifier
	} else {
		exit_error("Syntax error: Expected an identifier in <varList>.");
	}

	// Expect a comma
	if (current_token.id == OP_TK && current_token.instance == ",") {
		match(OP_TK); //consume ','
		var_list_node.add_child(create_node(","));
	} else {
        exit_error("Syntax error: Expected a ',' in <varList>.");
    }

	// Expect an integer
    if (current_token.id == NUM_TK) {
		var_list_node.add_child(create_node(current_token.instance));
		match(NUM_TK); //consume integer
	} else {
		exit_error("Syntax error: Expected an integer in <varList>.");
    }

	// Check if there is ';' or another <varList>
	if (current_token.id == OP_TK && current_token.instance == ";") {
		match(OP_TK); //consume ';'
		var_list_node.add_child(create_node(";"));
    } else {
		var_list_node.add_child(var_list());
    }

    return var_list_node;
}

//<block> -> start <vars> <stats> stop
Node Parser::block() {
	Node block_node = create_node("<block>");

	// Expect 'start' keyword
	if (current_token.id == KW_TK && current_token.instance == "start") {
		match(KW_TK); //consume 'start'
		block_node.add_child(create_node("start"));
	} else {
		exit_error("Syntax Error: Expected 'start' keyword at the beginning of <block>.");
	}

	block_node.add_child(vars()); // Parse <vars>
	block_node.add_child(stats()); // Parse <stats>

	// Expect 'stop' keyword
	if (current_token.id == KW_TK && current_token.instance == "stop") {
		match(KW_TK); //consume 'stop'
		block_node.add_child(create_node("stop"));
	} else {
		exit_error("Syntax Error: Expected 'stop' keyword at the end of <block>.");
	}

	return block_node;
}

//<stats> -> <stat>  <mStat>
Node Parser::stats() {
	Node stats_node = create_node("<stats>");

	stats_node.add_child(stat()); // Parse <stat>
	stats_node.add_child(m_stat()); // Parse <mStat>

	return stats_node;
}

//<mStat> -> empty | <stat>  <mStat>
Node Parser::m_stat() {
	Node m_stat_node= create_node("<mStat>");

	if (current_token.id == KW_TK && (current_token.instance == "read" || current_token.instance == "print" || current_token.instance == "iff" || current_token.instance == "iterate" || current_token.instance == "set" || current_token.instance == "start")) {
		m_stat_node.add_child(stat()); // Parse <stat>
		m_stat_node.add_child(m_stat()); // Parse <mStat>
	}

	return m_stat_node; // Return the constructed <mStat> node
}

//<stat> -> <read> | <print> | <block> | <cond> | <iter> | <assign>
Node Parser::stat() {
	Node stat_node = create_node("<stat>");

    if (current_token.id == KW_TK) {
        if (current_token.instance == "read") {
            stat_node.add_child(read()); // Parse <read>
        } else if (current_token.instance == "print") {
            stat_node.add_child(print()); // Parse <print> 
        } else if (current_token.instance == "start") {
            stat_node.add_child(block()); // Parse <block> 
        } else if (current_token.instance == "iff") {
            stat_node.add_child(cond()); // Parse <cond> 
        } else if (current_token.instance == "iterate") {
            stat_node.add_child(iter()); // Parse <iter> 
        } else if (current_token.instance == "set") {
            stat_node.add_child(assign()); // Parse <assign> 
        } else {
            exit_error("Syntax Error: Unexpected keyword in <stat>");
        }
    } else {
        exit_error("Syntax Error: Expected a statement keyword in <stat>");
    }

	return stat_node;
}
 
//<read> -> read identifier;
Node Parser::read() {
	Node read_node = create_node("<read>");

	// Expect 'read' keyword
	if (current_token.id == KW_TK && current_token.instance == "read") {
		match(KW_TK); //consume 'read'
		read_node.add_child(create_node("read"));
	} else {
		exit_error("Syntax error: Expected 'read' keyword in <read>.");
	}

	// Expect an identifier
	if (current_token.id == IDENT_TK) {
		read_node.add_child(create_node(current_token.instance));
		match(IDENT_TK); //consume identifier
	} else {
		exit_error("Syntax Error: Expected an identifier after 'read' in <read>.");
	}

	// Expect ';'
	if (current_token.id == OP_TK && current_token.instance == ";") {
		match(OP_TK); //consume ';'
		read_node.add_child(create_node(";"));
	} else {
		exit_error("Syntax Error: Expected ';' at the end of <read>.");
	}

	return read_node;
}

//<print> -> print <exp>;
Node Parser::print() {
	Node print_node = create_node("<print>");

	// Expect 'print' keyword
	if (current_token.id == KW_TK && current_token.instance == "print") {
		match(KW_TK); //consume 'print'
		print_node.add_child(create_node("print"));
	} else {
		exit_error("Syntax error: Expected 'print' keyword in <print>.");
	}

	print_node.add_child(exp()); // Parse <exp>

	// Expect ';'
	if (current_token.id == OP_TK && current_token.instance == ";") {
		match(OP_TK); //consume ';'
		print_node.add_child(create_node(";"));
	} else {
		exit_error("Syntax Error: Expected ';' at the end of <print>.");
	}

	return print_node;
}

//<cond> -> iff[<exp> <relational> <exp>] <stat>
Node Parser::cond() {
	Node cond_node = create_node("<cond>");

	// Expect 'iff' keyword
	if (current_token.id == KW_TK && current_token.instance == "iff") {
		match(KW_TK); //consume 'iff'
		cond_node.add_child(create_node("iff"));
	} else {
		exit_error("Syntax error: Expected 'iff' keyword in <cond>.");
	}

	// Expect '['
	if (current_token.id == OP_TK && current_token.instance == "[") {
		match(OP_TK); //consume '['
		cond_node.add_child(create_node("["));
	} else {
		exit_error("Syntax Error: Expected '[' after 'iff' in <cond>.");
	}

	cond_node.add_child(exp()); // Parse first <exp>

	cond_node.add_child(relational()); // Parse <relational>

	cond_node.add_child(exp()); // Parse second <exp>

	// Expect ']'
	if (current_token.id == OP_TK && current_token.instance == "]") {
		match(OP_TK); //consume ']'
		cond_node.add_child(create_node("]"));
	} else {
		exit_error("Syntax error: Expected ']' in <cond>.");
	}

	cond_node.add_child(stat()); // Parse <stat>

	return cond_node;
	
}

//<iter> -> iterate[<exp> <relational> <exp>]  <stat>
Node Parser::iter() {
	Node iter_node = create_node("<iter>");

	// Expect 'iterate' keyword
	if (current_token.id == KW_TK && current_token.instance == "iterate") {
		match(KW_TK); //consume 'iterate'
		iter_node.add_child(create_node("iterate"));
	} else {
		exit_error("Syntax error: Expected 'iterate' keyword in <iter>.");
	}

	// Expect '['
	if (current_token.id == OP_TK && current_token.instance == "[") {
		match(OP_TK); //consume '['
		iter_node.add_child(create_node("["));
	} else {
		exit_error("Syntax Error: Expected '[' after 'iterate' in <iter>.");
	}

	iter_node.add_child(exp()); // Parse first <exp>

	iter_node.add_child(relational()); // Parse <relational>

	iter_node.add_child(exp()); // Parse second <exp>

	// Expect ']'
	if (current_token.id == OP_TK && current_token.instance == "]") {
		match(OP_TK); //consume ']'
		iter_node.add_child(create_node("]"));
	}
	else {
		exit_error("Syntax error: Expected ']' in <iter>.");
	}

	iter_node.add_child(stat()); // Parse <stat>

	return iter_node;
}
 
//<assign> -> set identifier <exp>;
Node Parser::assign() {
	Node assign_node = create_node("<assign>");

	// Expect 'set' keyword
	if (current_token.id == KW_TK && current_token.instance == "set") {
		match(KW_TK); //consume 'set'
		assign_node.add_child(create_node("set"));
	} else {
		exit_error("Syntax error: Expected 'set' keyword in <assign>.");
	}

	// Expect an identifier
	if (current_token.id == IDENT_TK) {
		assign_node.add_child(create_node(current_token.instance));
		match(IDENT_TK); //consume identifier
	} else {
		exit_error("Syntax Error: Expected an identifier after 'set' in <assign>.");
	}

	assign_node.add_child(exp()); // Parse <exp>

	// Expect ';'
	if (current_token.id == OP_TK && current_token.instance == ";") {
		match(OP_TK); //consume ';'
		assign_node.add_child(create_node(";"));
	} else {
		exit_error("Syntax Error: Expected ';' at the end of <assign>.");
	}

	return assign_node;
}

//<relational> ..le. | .ge. | .lt. | .gt. | **| ~Note: these are 6 individual tokens
Node Parser::relational() {
	Node relational_node = create_node("<relational>");

	// Expect one of the relational operators
	if (current_token.id == OP_TK) {
		if (current_token.instance == ".le.") {
			relational_node.add_child(create_node(".le."));
			match(OP_TK); //consume '.le.'
		}
		else if (current_token.instance == ".ge.") {
			relational_node.add_child(create_node(".ge."));
			match(OP_TK); //consume '.ge.'
		}
		else if (current_token.instance == ".lt.") {
			relational_node.add_child(create_node(".lt."));
			match(OP_TK); //consume '.lt.'
		}
		else if (current_token.instance == ".gt.") {
			relational_node.add_child(create_node(".gt."));
			match(OP_TK); //consume '.gt.'
		}
		else if (current_token.instance == "**") {
			relational_node.add_child(create_node("**"));
			match(OP_TK); //consume '**'
		}
		else if (current_token.instance == "~") {
			relational_node.add_child(create_node("~"));
			match(OP_TK); //consume '~'
		} else {
			exit_error("Syntax Error: Expected a relational operator in <relational>.");
		}
	} else {
		exit_error("Syntax Error: Expected a relational operator in <relational>.");
	}

	return relational_node;
}

//<exp> -> <M> +<exp> | <M> -<exp> | <M>
Node Parser::exp() {
	Node exp_node = create_node("<exp>");
	exp_node.add_child(m()); // Parse <M>

	// Check for the '+' or '-' operators
	while (current_token.id == OP_TK && (current_token.instance == "+" || current_token.instance == "-")) {	
		string operator_instance = current_token.instance;

		match(OP_TK); // Consume the '+' or '-' operator

		if (operator_instance == "+" && current_token.id == OP_TK && current_token.instance == "+") {
			exit_error("Syntax Error: Expected an integer or an identifier after '+' in <exp>.");
		}

		exp_node.add_child(create_node(operator_instance)); 

		exp_node.add_child(m()); // Parse <M>
	}
	
	return exp_node; // Return the constructed <exp> node
}

//<M> -> <N> % <M> | <N>
Node Parser::m() {
	Node m_node = create_node("<M>");

	// Parse <N>
	m_node.add_child(n());

	// Check for the '%' operator
	while (current_token.id == OP_TK && current_token.instance == "%") {
		m_node.add_child(create_node("%")); // Add the operator %
		match(OP_TK); // Consume the '%' operator
		m_node.add_child(m()); // Parse <M>
	}

	return m_node;
}

//<N> ->   <R> / <N> | -<N> | <R>
Node Parser::n() {
	Node n_node = create_node("<N>");

	// Check for the unary '-' operator
	if (current_token.id == OP_TK && current_token.instance == "-") {
		n_node.add_child(create_node("-")); // Add the operator '-'
		match(OP_TK); // Consume the '-' operator
		n_node.add_child(n()); // Parse <N>
	} else {
		n_node.add_child(r()); // Parse <R>

		// Check for the '/' operator
		while (current_token.id == OP_TK && current_token.instance == "/") {
			n_node.add_child(create_node("/")); // Add the operator '/'
			match(OP_TK); // Consume the '/' operator

			// Check for the a valid token after '/'
			if (current_token.id != IDENT_TK && current_token.id != NUM_TK) {
				exit_error("Syntax Error: Expected an identifier or an integer after '/' in <N>.");
			}

			n_node.add_child(r()); // Parse <R>
		}
	}

	return n_node;
}

//<R>  -> (<exp>) | identifier | integer
Node Parser::r() {
	Node r_node = create_node("<R>");

	// Check for the '(' operator
	if (current_token.id == OP_TK && current_token.instance == "(") {
		match(OP_TK); // Consume the '(' operator
		r_node.add_child(create_node("(")); // Add the '(' operator

		// Parse <exp>
		r_node.add_child(exp());

		// Check for the ')' operator
		if (current_token.id == OP_TK && current_token.instance == ")") {
			match(OP_TK); // Consume the ')' operator
			r_node.add_child(create_node(")")); // Add the ')' operator
		} else {
			exit_error("Syntax Error: Expected ')' in <R>.");
		}
	}

	// Check for an identifier
	else if (current_token.id == IDENT_TK) {
		r_node.add_child(create_node(current_token.instance)); // Add the identifier
		match(IDENT_TK); // Consume the identifier
	}

	// Check for an integer
	else if (current_token.id == NUM_TK) {
		r_node.add_child(create_node(current_token.instance)); // Add the integer
		match(NUM_TK); // Consume the integer
	}
	
	return r_node; // Return the constructed <R> node
}
	
