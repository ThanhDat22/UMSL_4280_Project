#include "Token.h"

Token::Token(TokenID id, const string& instance, int line_num) : id(id), instance(instance), line_number(line_num) {}