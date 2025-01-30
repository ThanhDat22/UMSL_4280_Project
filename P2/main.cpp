#include "Parser.h"
#include "Tree.h"
#include "Utility.h"

#include <iostream>
#include <iostream>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::istringstream;

int main(int argc, char** argv) {
	string file_name;

	// Argument processing
	switch (argc) {
	case 1: {
		// Reads from the keyboard
		cout << "Reading from the keyboard" << endl;

		string user_input;
		string str = "";

		cout << "Enter the input: " << endl;

		// Collect user input
		do {
			getline(cin, user_input);
			if (!user_input.empty()) {
				str += user_input + "\n";
			}
		} while (!user_input.empty());

		// If input is empty, exit the program
		if (str.empty()) {
			exit_error("[Error] Empty input.");
		}

		// Parse the input
		istringstream iss(str);
		Parser parser(iss);
		Tree parse_tree = parser.parse();

		// Display the parse tree
		cout << "Parse tree constructed successfully from the keyboard input" << endl;
		cout << parse_tree.pre_order() << endl;

		break;
	} // End case 1

	case 2: {
		// Read from a file
		file_name = argv[1];


		ifstream fin;
		fin.open(file_name.c_str());

		// Check if the file is empty
		if (fin.peek() == ifstream::traits_type::eof()) {
			exit_error("[Error] Empty input file or file not found.");
			fin.close();
		}

		// Parse the file
		Parser parser(file_name);
		Tree parse_tree = parser.parse();

		// Display the parse tree
		cout << "Parse tree constructed successfully from the input file " << file_name << endl;
		cout << parse_tree.pre_order() << endl;

		fin.close();
		break;
	} // End case 2	
	default: {
		exit_error("[Error] Too many arguments provided");
		break;
	}

	} // End switch

	pause_program();
	return 0;
}