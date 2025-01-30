// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-28

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-03

#include "Token.h"

Token::Token(TokenID id, const string& instance, int line_num) : id(id), instance(instance), line_number(line_num) {}