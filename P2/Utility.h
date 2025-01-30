// Created by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-22

// Last updated by ThanhDat Nguyen (tnrbf@umsystem.edu) on 2024-10-22

#ifndef UTILITY_H
#define UTILITY_H

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>

using std::string;
using std::ifstream;

void exit_error(const string& s);// Display the warning the error
void pause_program(); // Pause the program and wait for user to press Enter
ifstream open_file(const string&); //Opens a file stream for reading
bool check_file_extension(const string&, const string&); // Check if the file has the correct extension

#endif