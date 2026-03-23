// Group 7) Joseph Barron, Gianna Martinez, Derek Schober, Andre Vershaw
#include "LexAnalyzer.h"


/*
 * Our objectives:
 * Change code to read line by line
 * When hitting an unknown symbol, write to output file UP TO the symbol, then throw error message
 * Fix identifiers so they check the space before and after them to make sure it's valid
 * Modularize with private methods to clean up scanFile method
 */

LexAnalyzer::LexAnalyzer(istream& infile) {
    string token;
    string lexeme;
    while (infile >> token >> lexeme) {
        tokenmap[lexeme] = token;
    }
}
void LexAnalyzer::UpdateVectors(string lexems, string token) {
    tokens.push_back(token);
    lexemes.push_back(lexems);
}

void LexAnalyzer::PrintToOutput(ostream &outfile) const {
    for (size_t i = 0; i < tokens.size(); i++) {
        outfile << tokens[i] << " " << lexemes[i] << endl;
    }
}

void LexAnalyzer::scanFile(istream& infile, ostream& outfile) {
    char ch;
    while (infile.get(ch)) {
        string lexeme;
        if (isspace(ch)) continue;
        if (isdigit(ch)) {
            lexeme += ch;
            while (isdigit(infile.peek())) {
                infile.get(ch);
                lexeme += ch;
            }

            UpdateVectors(lexeme, tokenmap[lexeme]);
            continue;
        }
        else if (isalpha(ch)) {
            lexeme += ch;
            while (isalnum(infile.peek()) || infile.peek() == '_') {
                infile.get(ch);
                lexeme += ch;
            }

            if (tokenmap.count(lexeme)) {
                UpdateVectors(lexeme, tokenmap[tokenmap[lexeme]]);

            }
            else {
                UpdateVectors(lexeme, "t_id");
            }
            continue;
        }

        else if (ch == '"') {
            while (infile.peek() != '"') {
                infile.get(ch);
                lexeme += ch;
            }
            if (infile.peek() == '"') {
                infile.get(ch);
                UpdateVectors(lexeme,"t_text");
            }
            else {
                cout << "Failed to define STRING" << endl;
                return;
            }
        }

        // SYMBOLS
        else {
            lexeme += ch;
            if (infile.peek() != EOF) {
                char tempNext = infile.peek();
                string twoChars = lexeme + tempNext;
                if (tokenmap.count(twoChars)) {
                    infile.get(ch);
                    lexeme = twoChars;
                }
            }
            if (tokenmap.count(lexeme)) {
                UpdateVectors(lexeme, tokenmap[lexeme]);
            }
            else {
                cout << "ERROR -> Unknown Symbol" << endl;
                cout << "THE SYMBOL IN QUESTION: " << lexeme << endl;
                return;
            }
        }
    }
    PrintToOutput(outfile);
    cout << "It runs!" << endl;
}
