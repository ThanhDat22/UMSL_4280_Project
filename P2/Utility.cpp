// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-22

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-22

#include "Utility.h"

using std::cout;
using std::cin;
using std::endl;


/** Displays an error message and exits the program.
    @param s: the error message to display to the user
*/
void exit_error(const string& s) {
	cout << s << endl;
	cout << "\nPress Enter to continue...";
	cin.get();
	exit(1);
}

// Pause the program and wait for user to press Enter
void pause_program(){
    cout << "\nPress Enter to continue..." << endl;
    cin.get(); // Waits for the user to press "Enter"
}

/** Opens a file stream for reading.
    @param file_name: the name of the file to open
    @return: an ifstream object for the opened file
    @throws: exits the program with an error message if the file cannot be opened
*/
// ifstream open_file(const string& file_name) {
// 	ifstream fin;
// 	fin.open(file_name.c_str());
// 	if(!fin) {
// 		exit_error("[Error] Nonexistent input file.");
// 	}
// 	return fin;
// }

/** Checks if the given filename has the expected extension.
    @param filename: the name of the file to check
    @param expected_extension: the expected file extension (e.g., ".4280fs24")
    @return: true if the file has the expected extension, false otherwise
*/
bool check_file_extension(const string& filename, const string& expected_extension) {
    size_t extension_pos = filename.rfind(expected_extension);
    return (extension_pos != string::npos && extension_pos == filename.length() - expected_extension.length());
}