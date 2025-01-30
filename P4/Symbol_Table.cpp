#include "Symbol_Table.h"

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::make_pair;
using std::pair;
using std::ostringstream;


// Constructor
Symbol_Table::Symbol_Table() {
    enter_scope(); // Start with a global scope
}

// Member functions

// Exit the current scope
void Symbol_Table::enter_scope() {
    symbol_table.push(map<string, bool>());
}

// Exit the current scope
void Symbol_Table::exit_scope() {
    if (!symbol_table.empty()) {
        symbol_table.pop();
    } else {
        error_message("ERROR in static semantics: No scope to exit. \n");
    }
}

/** Convert a size_t to a string
 *  @param num The number to convert
 *  @return The converted string
 */
// string Symbol_Table::to_string(const size_t num) {
//     ostringstream oss;
//     oss << num;
//     return oss.str();
// }

/** Insert a variable into the symbol table
 *  @param variable The variable to insert
 *  @param line_number The line number of the variable
 * @return True if the variable is inserted, false otherwise
 */
bool Symbol_Table::insert(const string& variable, size_t line_number) {
    if (symbol_table.empty()) {
        enter_scope(); // Create a new scope if the symbol table is empty
    }

    map<string, bool>& current_scope = symbol_table.top();

    if (current_scope.find(variable) != current_scope.end()) {
        error_message("ERROR in static semantics: Variable '" + variable + "' redefined at line " + to_string(line_number));
        return false;
    } else {
        current_scope[variable] = false; // Mark as unused
        return true;
    }
}


/** Check the unused variables in the symbol table
 *  @param variable The variable to verify
 *  @param line_number The line number of the variable
 *  @return True if the variable exists, false otherwise
 */
bool Symbol_Table::verify(const string& variable, size_t line_number) {
    stack<map<string, bool> >& symbol_stack = symbol_table; // Directly use symbol_table

    while (!symbol_stack.empty()) {
        map<string, bool>& current_scope = symbol_stack.top();

        if (current_scope.find(variable) != current_scope.end()) {
            current_scope[variable] = true; // Mark as used in the actual symbol_table
            return true;
        }

        symbol_stack.pop();
    }

    error_message("ERROR in static semantics: Variable '" + variable + "' used without declaration at line " + to_string(line_number));
    return false;
}

// Check the unused variables in the symbol table
void Symbol_Table::check_variable() {
    if (symbol_table.empty()) return;

    // Check the top scope for unused variables
    map<string, bool>& current_scope = symbol_table.top();

    for (map<string, bool>::const_iterator it = current_scope.begin(); it != current_scope.end(); ++it) {
        if (!it->second) {
            cerr << "WARNING: Variable '" << it->first << "' declared but not used.\n";
        }
    }
}

/** Reports an error message
 *  @param message The error message to report
 */
void Symbol_Table::error_message(const string& message) {
    cerr << message << endl;
    exit(1);
}

/** Reports a warning message
 *  @param message The warning message to report
 */
void Symbol_Table::warning_message(const string& message) {
    cout << message << endl;
}

// Get the variables in the symbol table
vector<string> Symbol_Table::get_vars() const {
    vector<string> vars;

    for (stack<map<string, bool> > symbol_stack = symbol_table; !symbol_stack.empty(); symbol_stack.pop()) {
        const map<string, bool>& current_scope = symbol_stack.top();

        for (map<string, bool>::const_iterator it = current_scope.begin(); it != current_scope.end(); ++it) {
            vars.push_back(it->first);
        }
    }

    return vars;
}

// Check if the symbol table contains a variable
bool Symbol_Table::contains(const string& variable) const {
    stack<map<string, bool> > symbol_stack = symbol_table; // Directly use symbol_table

    while (!symbol_stack.empty()) {
        const map<string, bool>& current_scope = symbol_stack.top();

        if (current_scope.find(variable) != current_scope.end()) {
            return true;
        }

        symbol_stack.pop();
    }

    return false;
}