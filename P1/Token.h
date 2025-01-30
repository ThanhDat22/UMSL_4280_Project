#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

using std::string;
using std::vector;

// Enum for token ID
enum TokenID { IDENT_TK, NUM_TK, KW_TK, OP_TK, EOF_TK, ERROR_TK };

// Struc of Token
struct Token {
    TokenID id;
    string instance;
    int line_number;

    // Constructors
    Token(TokenID = ERROR_TK, const string& = " ", int = 0);
};

#endif //TOKEN_H
