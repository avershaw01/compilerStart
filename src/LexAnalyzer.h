#ifndef LEXANALYZER_H
#define LEXANALYZER_H
// above can also be achieved with #pragma once

// Group 7) Joseph Barron, Gianna Martinez, Derek Schober, Andre Vershaw

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;
class LexAnalyzer {
    private:
        // map to hold language's valid lexeme/token pairs
        map<string, string> tokenmap;

        // parallel vectors to store valid lexemes and tokens from source code file
        vector<string> lexemes;
        vector<string> tokens;

        // other private methods
    void UpdateVectors(string lexeme, string token);
    void PrintToOutput(ostream &outfile,bool& errorFlag,string& errorMsg) const;
    void CheckSymbols(int &i, string &line, string &lexeme, bool &errorFlag, string &errorMsg);
    void CheckIDandKeyWords(int& i, string &line, string &lexeme, bool &errorFlag, string &errorMsg);
    void CheckString(int& i, string &line, string &lexeme, bool &errorFlag, string &errorMsg, istream &infile);
    public:
        LexAnalyzer(istream& infile);
        void scanFile(istream& infile, ostream& outfile);
};
#endif //LEXANALYZER_H
