Option 2:

Project name: Scanner
implemented using C++ language.

Description:
This is a scanner project that is implemented using option 2 for the provided lexical definitions:
Case sensitive
Each scanner error should display "LEXICAL ERROR: " followed by details including the line number if counted
Alphabet
    all English letters (upper and lower), digits, plus the extra characters as seen below, plus white spaces
    No other characters allowed and they should generate errors
Identifiers
    begin with any letter
    continue with any number of letters and digits and underscores, up to 8 significant total
        you may assume no identifier is longer than 8 characters (in testing, this is intended to make it easier not harder)
Keywords (reserved)
    start stop iterate var exit read print iff then set func program
Operators and delimiters (single or double character, can produce individual tokens or as a group with instances)
    =  .le.  .ge. .lt. .gt. ~ : ;  +  -  **  /  %   (  ) , { } ; [ ]
    Note the mult-char tokens, without spaces
Integers
    any sequence of decimal digits, no sign, no decimal point, up to 8 significant
    you may assume no number longer than 8 characters (in testing)
Comments start with @@ and end with @ and contain any characters
    you may assume no white spaces inside as in @@thisiscomment@ especially if implementing option 1
    you may also assume no \n inside

Files:
    Token.h: Define the Token struct and enum for tokenID
    Token.cpp: define Token constructor
    Scanner.h: Header file for the Scanner class
    Scanner.cpp: Implementation of the Scanner class.
    Test_Scanner.h: Header file for the test_Scanner 
    Test_Scanner.cpp: Implementation for the test function to scan the input.
    main.cpp: the starting point for program execution; handling arguments processing
    README.txt
    Makefile: to compile all files to run the program
    P1_test1.ext P1_test2.ext P1_test3.ext P1_test4.ext P1_test5.ext P1_test6.ext P1_test7.ext

Running instruction:
    Compile the program using the command: make
    Run the program: ./P1 
                or: ./P1 [somefile]
