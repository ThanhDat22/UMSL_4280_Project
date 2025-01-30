#include "Binary_Search_Tree.h"
#include "Binary_Tree.h"
#include "P0.h"

#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

int main(int argc, char* argv[]) {

	//Variable declaration
	string file_name;
	
	vector<string> data;
	Binary_Tree<string> tree;

	//Arguments processing
	switch(argc) {
	case 1: {
		cout << "Reading from the keyboard." << endl;
		string user_input;
		cout << "Please enter a using keyboard." << endl;
		do {
			getline(cin, user_input);
			vector<string> words = split_sententce(user_input);
			for (size_t i = 0; i < words.size(); i++) {
				if (is_all_letter(words.at(i))) { data.push_back(to_lower_case(words.at(i))); }
				else { exit_error("[E]RROR! Input data should be all strings"); }
			}
		} while (!user_input.empty());

		tree = build_tree(data);
		
		ofstream print_preorder("out.preorder");
		ofstream print_inorder("out.inorder");
		ofstream print_postorder("out.postorder");

		print_preorder << tree.pre_order();
		print_inorder << tree.in_order();
		print_postorder << tree.post_order();
		
		// Close output file
		print_preorder.close();
		print_inorder.close();
		print_postorder.close();

		break;				
	} // end of case 1

	case 2: {
		cout << "Need to read from file..." << endl << endl;
		
		file_name = argv[1];
		ifstream fin;

		size_t extension = file_name.find(".");

		//Check if there is extension.
		if (extension == string::npos) {
			exit_error("Improper format.Expected file extension");
		}
		
		//Open the input file.
		fin.open(file_name.c_str());

		//Check whether the input file was successfully opened.
		if (!fin) {
			exit_error("[E]rror Nonexitstent input file");
		}

		//Open the output file
		string file_name_front = get_file_name(file_name);
		
		string preorder_out = file_name_front + ".preorder";
		string inorder_out = file_name_front + ".inorder";
		string postorder_out = file_name_front + ".postorder";

		ofstream print_preorder;
		ofstream print_inorder;
		ofstream print_postorder;

		print_preorder.open(preorder_out.c_str());
		print_inorder.open(inorder_out.c_str());
		print_postorder.open(postorder_out.c_str());

		string st;
		while (fin >> st) {
			if (is_all_letter(st)) { data.push_back(to_lower_case(st)); }
			else { exit_error("[E]RROR! Input data should be all strings"); }
		}

		tree = build_tree(data);

		print_preorder << tree.pre_order();
		print_inorder << tree.in_order();
		print_postorder << tree.post_order();

		// Close output file
		print_preorder.close();
		print_inorder.close();
		print_postorder.close();
		fin.close();

		break;
	} // end of case 2

	default:
		exit_error("[E]RROR! Too many arguments!");
	} // end of switch 
	return 0;
}


