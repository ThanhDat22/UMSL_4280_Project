#include "Test_Scanner.h"

#include <cstdlib>

using std::endl;
using std::cout;
using std::cin;

void test_scanner(istream& input) {
	Scanner scanner(input);
	Token token;

	while (true) {
		token = scanner.get_next_token();
		if (token.id == EOF_TK) {
			cout << "End of file reached" << endl;
			break;
		}
		cout << "Line: " << token.line_number << "\t" << Scanner::get_tkid_name(token.id) << " \tToken: " << token.instance << endl;
	}
}

// Display the warning the error
void exit_error(const string& s) {
	cout << s << endl;
	cout << "\nPress Enter to continue...";
	cin.get();
	exit(1);
}

