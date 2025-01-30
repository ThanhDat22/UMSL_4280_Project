#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include "Token.h"
#include "Node.h"
#include "Tree.h"
#include "Utility.h"

#include <string>
#include <sstream>

using std::string;
using std::ostringstream;
using std::istringstream;

class Parser {
public:
	// Constructor
	Parser(const string& ="");
	Parser(istringstream&); // Constructor for parsing from a string stream

	// Member functions
	Tree parse(); // Parse the input file and return the parse tree
	Node program(); // <program>  ->     program <vars> <block>

private:

	// Data fields
	string file_name; // Name of the file to be parsed
	Token current_token; // Current token
	ifstream fin; // Input file stream
	Scanner scanner; // Scanner object to fetch tokens

	// Member functions
	void next_token(); // Fetch the next token
	void match(TokenID); // Match the current token with the expected token ID
	
	
	Node vars(); // <vars>         ->      empty | var <varList>
	Node var_list(); // <varList>     ->      identifier , integer ; | identifier , integer <varList>
	Node block(); // <block>       ->      start <vars> <stats> stop
	Node stats(); // <stats>         ->      <stat>  <mStat>
	Node m_stat(); // <mStat>        ->      empty | <stat> <mStat>
	Node stat(); // <stat>           ->      <read> | <print> | <block> | <cond> | <iter> | <assign>
	Node read(); // <read>         ->      read identifier ;
	Node print(); // <print>        ->     print <exp> ;
	Node cond(); // <cond>        ->      iff [ <exp> <relational> <exp> ] <stat>
	Node iter(); // <iter>           ->      iterate [ <exp> <relational> <exp> ]  <stat>
	Node assign(); // <assign>      ->     set identifier <exp> ;
	Node exp(); //  <M> + <exp> | <M> - <exp> | <M>
	Node relational(); // .le. | .ge. | .lt. | .gt. | **| ~Note: these are 6 individual tokens
	Node m(); // <M>             ->      <N> % <M> | <N>
	Node n(); // <N>             ->      <R> / <N> | - <N> |  <R>
	Node r(); // <R>              ->      ( <exp> )  | identifier | integer  
};

#endif // !PARSER_H


