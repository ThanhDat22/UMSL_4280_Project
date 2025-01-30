#include "Test_Scanner.h"

#include <sstream> // For istringstream

using namespace std;

int main(int argc, char** argv) {
	string filename = "test.txt";

	// Arguments processing
	switch (argc) {
	case 1: {
		cout << "Reading from the keyboard." << endl;
		string user_input;
		string str = "";
		cout << "Please enter a using keyboard." << endl;
		do {
			getline(cin, user_input);
			str += user_input + "\n"; // Adds a new line for each input
		} while (!user_input.empty());
		istringstream iss(str);
		test_scanner(iss);

		cout << "\nPress Enter to continue...";
		cin.get();

		break;
	}
	case 2: {
		cout << "Need to read from file..." << endl << endl;

		filename = argv[1];
		ifstream fin;

		size_t extension = filename.find(".");

		//Check if there is extension.
		if (extension == string::npos) {
			exit_error("Improper format. Expected file extension");
		}

		//Open the input file.
		fin.open(filename.c_str());

		//Check whether the input file was successfully opened.
		if (!fin) {
			exit_error("[E]rror Nonexitstent input file");
		}
		test_scanner(fin);
		fin.close();
		break;
	}
	default:
		exit_error("[E]RROR! Too many arguments!");
	}
	return 0;

}