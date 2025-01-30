// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-11-07

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-12-10

#include "Parser.h"
#include "Tree.h"
#include "Static_Semantics.h"
#include "Utility.h"
#include "Generator.h"

#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::string;

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

        // Perform static semantics checks
        Static_Semantics semantics(parse_tree, iss);
        semantics.check_semantics();

        // Generate code
        Generator generator(parse_tree);

        // Output the generated code
        ofstream fout;
        string filename = "a.asm";
        fout.open(filename.c_str());

        fout << generator.get_code() << endl;
        fout.close();

        cout << "Generated code has been written to " << filename << endl;

        break;
    }

    case 2: {
        // Read from a file
        file_name = argv[1];
        string file = file_name + ".4280fs24";

        ifstream fin;
        fin.open(file.c_str());

        // Check if the file is empty
        if (fin.peek() == ifstream::traits_type::eof()) {
            exit_error("[Error] Empty input file.");
            fin.close();
        }

        // Parse the input
        Parser parser(file);
        Tree parse_tree = parser.parse();

        // Create a Scanner for Static_Semantics
        Scanner scanner(fin);
        Static_Semantics semantics(parse_tree, fin);
        semantics.check_semantics();

        // Generate code
        Generator generator(parse_tree);
        generator.generate();

        // Write the generated code to a file
        string output_file = file_name + ".asm";

        ofstream fout;
        fout.open(output_file.c_str());

        fout << generator.get_code() << endl;

        cout << "Generated code has been written to " << output_file << endl;

        fout.close();
        fin.close();
        break;
    }

    default:
        exit_error("[Error] Too many arguments provided");
        break;
    }

    //pause_program();
    return 0;
}

