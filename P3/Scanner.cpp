#include "Scanner.h"

#include <iostream>
#include <cctype>
#include <cstdio> // For EOF

using std::find;

// Constructor
Scanner::Scanner(istream& input_stream) : in_stream(input_stream), crr_line_num(1) { initialize_keywords(); }


// Classes member functions

// Initialize the keywords vector
void Scanner::initialize_keywords() {
    keywords.push_back("start");
    keywords.push_back("stop");
    keywords.push_back("iterate");
    keywords.push_back("var");
    keywords.push_back("exit");
    keywords.push_back("read");
    keywords.push_back("print");
    keywords.push_back("iff");
    keywords.push_back("then");
    keywords.push_back("set");
    keywords.push_back("func");
    keywords.push_back("program");
}

// Checks if a string is keyword
bool Scanner::is_keyword(const string& str) {
    for (size_t i = 0; i < keywords.size(); i++) {
        if (keywords.at(i) == str) { return true; }
    }
    return false;
}

// Skips whitespace and comments
void Scanner::skip_whitespace_cmments() {
    char c;
    while (in_stream.get(c)) {
        if (isspace(static_cast<unsigned char>(c))) { // Cast to unsigned char to prevent assertion failure
            if (c == '\n') {
                crr_line_num++; // Increat line count for newlines
            }
            continue;
        }

        // Handls comment: @@comment@
        if (c == '@') {
            char next_ch;
            in_stream.get(next_ch);
            if (next_ch == '@') {
                
                // Skip letters until the closing '@' of comment
                while (in_stream.get(c) && c != '@') {}
                continue; // Skip after the comment ends.
            }
            else {
                in_stream.putback(next_ch);
                in_stream.putback(c);
                break; // break if there is not a comment
            }
        }
        else {
            in_stream.putback(c); // if there is not a whitespace or comment, put the letter back
            break;
        }
    }
}

// Gets the next token
Token Scanner::get_next_token() {
    skip_whitespace_cmments();

    // Check if reacheed the end of the file
    if (in_stream.peek() == EOF) {
        return Token(EOF_TK, "EOF", crr_line_num);
    }

    string inst_token;
    char c;
    
    // Get next character
    in_stream.get(c);

    // Check if EOF was encountered
    if (in_stream.eof()) { return Token(EOF_TK, "EOF", crr_line_num); }

    // Check for identifiers or keywords
    if (isalpha(static_cast<unsigned char>(c))) { // Cast to unsigned char to prevent assertion failure
        inst_token += c;
        while (in_stream.get(c) && (isalnum(static_cast<unsigned char>(c)) || c == '_')) { inst_token += c; }
        in_stream.putback(c); // Put the non-identifier letter back

        if (is_keyword(inst_token)) {
            return Token(KW_TK, inst_token, crr_line_num);
        }
        else {
            return Token(IDENT_TK, inst_token, crr_line_num);
        }
    }

    // Check for numbers
    if (isdigit(static_cast<unsigned char>(c))) {
        inst_token += c;
        while (in_stream.get(c) && isdigit(static_cast<unsigned char>(c))) { inst_token += c; }
        in_stream.putback(c); // Put the non-digit character back
        return Token(NUM_TK, inst_token, crr_line_num);
    }

    // Check for operators or delimiters
    inst_token += c;
    if (c == '~' || c == ':' || c == ';' || c == '+' || c == '-' || c == '/' || c == '%' ||
        c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == '=')  {
        return Token(OP_TK, inst_token, crr_line_num);
    }

    if (c == '*') {
        char next_ch;
        in_stream.get(next_ch);
        if (next_ch == '*') {
            inst_token += next_ch;
            return Token( OP_TK, inst_token, crr_line_num);
        }
        in_stream.putback(next_ch); // Put back the last non-matching charracter
    }
    if (c == '.') {
        char next_ch;
        in_stream.get(next_ch);
        if (next_ch == 'l' || next_ch == 'g') {
            inst_token += next_ch;
            in_stream.get(next_ch);
            if (next_ch == 'e' || next_ch == 't') {
                inst_token += next_ch;
                in_stream.get(next_ch);
                if (next_ch == '.') {
                    inst_token += next_ch;
                    return Token(OP_TK, inst_token, crr_line_num);
                }
            }
        }
        in_stream.putback(next_ch); // Put back the last non matching character
    }
    
    // If no valid token is found
    in_stream.get(c);
    return Token(ERROR_TK, "LEXICAL ERROR: Invalid token is found", crr_line_num);
}

// Gets name of Token ID
string Scanner::get_tkid_name(TokenID id) {
    switch (id) {
    case IDENT_TK: return "Identifier";
    case NUM_TK: return "Number";
    case KW_TK: return "Keyword";
    case OP_TK: return "Operator";
    case EOF_TK: return "End of File";
    case ERROR_TK: return "Error";
    default: return "Unknow";
    }
}
