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
LexAnalyzer::LexAnalyzer(istream &infile) {
    string token;
    string lexeme;
    while (infile >> token >> lexeme) {
        tokenmap[lexeme] = token;
    }
}

void LexAnalyzer::UpdateVectors(string lexeme, string token) {
    tokens.push_back(token);
    lexemes.push_back(lexeme);
}

void LexAnalyzer::PrintToOutput(ostream &outfile,bool& errorFlag,string& errorMsg) const {
    for (size_t i = 0; i < tokens.size(); i++) {
        outfile << tokens[i] << " " << lexemes[i] << endl;
    }
    if (errorFlag) {
        outfile << errorMsg << endl;
    }
}

void LexAnalyzer::CheckSymbols(int &i, string &line, string &lexeme, bool &errorFlag, string &errorMsg) {
    if (i + 1 < line.length()) {
        // End of File
        string twoChars = lexeme + line[i + 1];
        // twoChars variable is used to see if the pair is a valid symbol

        if (tokenmap.count(twoChars)) {
            lexeme = twoChars;
            i++;
        }
    }
    if (tokenmap.count(lexeme)) {
        UpdateVectors(lexeme, tokenmap[lexeme]);
    } else {
        // This should only run if the compiler reaches an unknown symbol
        // All other cases; letters, numbers, and valid symbols; should continue; (skips to next iteration)
        // cout << "ERROR -> Unknown Symbol" << endl;

        // DEBUG to print to console what symbol broke the lexeme analyzer:
        // cout << "THE SYMBOL IN QUESTION: " << lexeme << endl;
        errorFlag = true;
        errorMsg = "THE SYMBOL IN QUESTION: " + lexeme;
    }
}

void LexAnalyzer::CheckIDandKeyWords(int& i, string &line, string &lexeme, bool &errorFlag, string &errorMsg) {
    char ch;
    while (i < line.length() && ( isalnum(line[i+1]) || line[i+1] == '_')) {
        ch = line[++i];
        lexeme += ch;
    }

    if (tokenmap.contains(lexeme)) {
        UpdateVectors(lexeme, tokenmap[lexeme]);
    } else {
        UpdateVectors(lexeme, "t_id");
    }
}
void LexAnalyzer::CheckString(int& i, string &line, string &lexeme, bool &errorFlag, string &errorMsg, istream &infile) {
    char ch;
    if (i > 0 && isalnum(line[i-1])) {
        errorFlag = true;
        errorMsg = "UNEXPECTED QUOTATION: " + line;
        return;
    }

    ch = '_';
    i++;

    while (ch != '"') {
        if (i < line.length()) {
            ch = line[i];
        }
        else {
            std::getline(infile, line);
            i = 0;
            if (line.empty()) {
                lexeme += '\n';
                continue;
            }

            ch = line[i];
        }

        if (ch != '"' && ch != '\0') {
            lexeme += ch;
            i++;
        }

    }
    UpdateVectors(lexeme, "t_text");
    if (i < line.length() && isalnum(line[i])) {
        errorFlag = true;
        errorMsg = "Unexpected character: " + line;
    }
}
void LexAnalyzer::scanFile(istream &infile, ostream &outfile) {
    // While loops through input file source.txt
    string line;
    bool errorFlag = false;
    string errorMsg;

    while (getline(infile, line)) {
        int length = line.length();
        int i = -1;
        while ((++i < length) && !errorFlag) {
            // Instantiates temp variable to hold the lexeme
            string lexeme;
            char ch = line[i];

            // Ignores whitespace
            if (isspace(ch)) {
                continue;
            }
            // Checks NUMBERS first:
            if (isdigit(ch)) {
                lexeme += ch;
                while (i + 1 < line.length() && isdigit(line[i + 1]) && !errorFlag) {
                    lexeme += line[++i];
                }
                if (i + 1 < line.length() && (isalpha(line[i + 1]) || line[i + 1] == '_')) {
                    errorFlag = true;
                    errorMsg = "No Delimiter Between number and words: " + line;
                }
                if (!errorFlag) {
                    UpdateVectors(lexeme, "t_number");
                }
            }

            // Now checking ALPHABET characters: (NOT a String, or else first char would be ")
            else if (isalpha(ch) || ch == '_') {
                // Variables cannot start with a number
                lexeme += ch;
                CheckIDandKeyWords(i,line,lexeme,errorFlag,errorMsg);
            }
            // Checking for STRINGS (double quotes)
            else if (ch == '"') {
                CheckString(i,line,lexeme,errorFlag,errorMsg,infile);
            }
            // SYMBOLS
            else {
                lexeme += ch;
                CheckSymbols(i, line, lexeme,errorFlag, errorMsg);
            }
        }
    }
    // If no errors ran return; pushes to output file what was read (using the parallel vectors)
    PrintToOutput(outfile,errorFlag,errorMsg);

    if (!errorFlag)
        cout << "It runs!" << endl; // DEBUG to console, ensures this section ran.
    else
        cout << "Error -- " << errorMsg << endl;
}