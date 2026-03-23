// Group 7) Joseph Barron, Gianna Martinez, Derek Schober, Andre Vershaw
#include "LexAnalyzer.h"


/*
 * Our objectives:
 * Change code to read line by line
 * When hitting an unknown symbol, write to output file UP TO the symbol, then throw error message
 * Fix identifiers so they check the space before and after them to make sure it's valid
 * Modularize with private methods to clean up scanFile method
 */

// pre: parameter refers to open data file consisting of token and
// lexeme pairs i.e.  t_and and
// Each pair appears on its own input line.
// post: tokenmap has been populated - key: lexeme, value: token

LexAnalyzer::LexAnalyzer(istream& infile) {
    // CONSTRUCTOR

    // Build tokenmap from the infile given.
    /*
     * Create temporary variables, one for token, one for lexeme
     * Loop through file until it ends
     * Each iteration reads one pair
     * Inserts into tokenmap
     */

    // Instantiate token and lexeme temp variables:
    string token;
    string lexeme;

    // Loops through lexemes.txt file, builds tokenmap
    while (infile >> token >> lexeme) {
        // cout << lexeme << " -> " << token << endl; // my DEBUG
        tokenmap[lexeme] = token;
    }
}

// pre: 1st parameter refers to an open text file that contains source
// code in the language, 2nd parameter refers to an open empty output
// file
// post: If no error, the token and lexeme pairs for the given input
// file have been written to the output file (token : lexeme).
// If there is an error, the incomplete token/lexeme pairs, as well as
// an error message have been written to the output file.
// A success or fail message has printed to the console.
void LexAnalyzer::scanFile(istream& infile, ostream& outfile) {
    // Reads through source infile (code to compile), builds output file using tokenmap,
    /*
     *lexemes.push_back(...)
     *tokens.push_back(...)
     */

    // Instantiates temp char variable 'ch'
    char ch;

    // While loops through input file source.txt
    while (infile.get(ch)) {
        // Instantiates temp variable to hold the lexeme
        string lexeme;

        // Ignores whitespace
        if (isspace(ch)) continue;

        // Checks NUMBERS first:
        if (isdigit(ch)) {
            lexeme += ch;
            // Uses infile.peek() to group numbers together
            while (isdigit(infile.peek())) {
                infile.get(ch);
                lexeme += ch;
            }

            // Adds to parallel vectors:
            tokens.push_back("t_number");
            lexemes.push_back(lexeme);
            continue;
        }

        // Now checking ALPHABET characters: (NOT a String, or else first char would be ")
        else if (isalpha(ch)) {
            // Variables cannot start with a number
            lexeme += ch;

            // isalnum returns true if the char is a number OR letter
            while (isalnum(infile.peek()) || infile.peek() == '_') {
                infile.get(ch); // Reads next char, stores it in ch, moves reader to the next char.
                lexeme += ch;
            }

            if (tokenmap.count(lexeme)) {
                tokens.push_back(tokenmap[lexeme]); // If the lexeme matches a defined token, pushes that token
            }
            else {
                tokens.push_back("t_id"); // Else, lexeme is an identifier (variable name)
            }
            lexemes.push_back(lexeme);
            continue;
        }

        // Checking for STRINGS (double quotes)
        else if (ch == '"') {
            // Only the data within the ""s is stored in the output file, "s are never added to the lexeme
            while (infile.peek() != '"') { // Continues until closing "
                infile.get(ch);
                lexeme += ch;
            }
            if (infile.peek() == '"') {
                infile.get(ch);
                tokens.push_back("t_text");
                lexemes.push_back(lexeme);
            }
            else {
                cout << "Failed to define STRING" << endl; // String is never closed
                return;
            }
        }

        // SYMBOLS
        else {
            lexeme += ch;

            if (infile.peek() != EOF) { // End of File
                char tempNext = infile.peek();
                string twoChars = lexeme + tempNext; // twoChars variable is used to see if the pair is a valid symbol

                if (tokenmap.count(twoChars)) {
                    infile.get(ch);
                    lexeme = twoChars;
                }
            }
            if (tokenmap.count(lexeme)) {
                tokens.push_back(tokenmap[lexeme]);
                lexemes.push_back(lexeme);
            }
            else { // This should only run if the compiler reaches an unknown symbol
                // All other cases; letters, numbers, and valid symbols; should continue; (skips to next iteration)
                cout << "ERROR -> Unknown Symbol" << endl;

                // DEBUG to print to console what symbol broke the lexeme analyzer:
                cout << "THE SYMBOL IN QUESTION: " << lexeme << endl;
                return;
            }
        }
    }
    // If no errors ran return; pushes to output file what was read (using the parallel vectors)
    for (size_t i = 0; i < tokens.size(); i++) {
        outfile << tokens[i] << " " << lexemes[i] << endl;
    }
    cout << "It runs!" << endl; // DEBUG to console, ensures this section ran.
}
