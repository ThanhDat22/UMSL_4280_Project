#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"

#include <istream>

using std::istream;

class Scanner {
public:
    // Constructor
    
    Scanner(istream& in);
    Token get_next_token();
    bool is_keyword(const string&); // Checks if a string is keyword
    static string get_tkid_name(TokenID);
private:

    istream& in_stream; // Input stream
    int crr_line_num; // Current line number

    vector<string> keywords; // A vector contain keywords

    void skip_whitespace_cmments(); // Skips whitespace and comments
    void initialize_keywords();
};

#endif

