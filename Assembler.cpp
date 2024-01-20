#include <algorithm>  // STL algorithms like sort, search, copy, remove element...
#include <fstream>    // read write files
#include <iostream>   // display debug information in terminal
#include <queue>      //internal buffer between each pass
#include <string>     //instruction are saved as C++ stings.

#include "Parser.hpp"
#include "SymbolTable.hpp"

using namespace std;

SymbolTable MyST;
Parser MyParser;

int main(int argc, char const *argv[]) {
    string fnameIn, fnameOut;
    ifstream fIn;
    ofstream fOut;

    queue<string> queue1, queue2;  // tempory asm instruction storage

    /* handling parameters*/
    if (argc < 2) {
        throw runtime_error("main(): plseas specify input file MyAssembler *.asm [*.hack]");
    } else {
        fnameIn = string(argv[1]);
        // check the input file is a .asm?
        if (fnameIn.find(".asm") == string::npos)
            throw runtime_error("main(): input fime must be .asm");
        fIn.open(fnameIn);
        // check the specified file is able to open?
        if (!fIn.is_open()) {
            throw runtime_error("main(): unable to open file " + fnameIn);
        }

        if (argc == 2) {
            string::size_type idx = fnameIn.find_last_of('.');  // find the base name
            fnameOut = fnameIn.substr(0, idx) + ".hack";        // change to .hack
        } else if (argc == 3) {
            fnameOut = string(argv[2]);
        } else {
            throw runtime_error("main(): too many parameters");
        }
        fOut.open(fnameOut);
        if (!fOut.is_open()) {
            throw runtime_error("main(): unable to open file " + fnameOut);
        }

        cout << "output to " << fnameOut << endl;

        /* Pre pass: read source file clean-up the code */
        cout << "\t======\tPre-pass\t=====\t" << endl;
        string line;
        /* read each line from the .asm file */
        while (getline(fIn, line)) {
            cout << line << endl;
            /* remove comments */
            string::size_type idx = line.find("//");  // find start of "//"
            string lineRmComm = line.substr(0, idx);
            if (lineRmComm.size() == 0) continue;  // skip empty line
            /* remove spaces */
            string::iterator str_iter = remove(lineRmComm.begin(), lineRmComm.end(), ' ');
            lineRmComm.erase(str_iter, lineRmComm.end());
            queue1.push(lineRmComm);  // push to queue
        }

        /* First pass read lables build symbol table*/
        cout << "\t======\tFirst-pass\t======\t" << endl;
        while (!queue1.empty()) {
            line = queue1.front();
            queue1.pop();
            cout << line << endl;
            /* Is this line a L-instruction (Label) */
            if (line.find('(') != string::npos && line.find(')') != string::npos) {
                string::size_type idx_L = line.find_last_of('(');
                string::size_type idx_R = line.find_first_of(')');
                line = line.substr(idx_L + 1, idx_R - 1);
                MyST.addLabel(line);
                continue;  // skip the push to the queue2
            } else {
                MyST.incLabelCounter();  // move the label counter to the next line (ROM Address)
            }
            queue2.push(line);
        }

        /* Second pass Translate instructions */
        cout << "\t======\tSecond-pass\t======\t" << endl;
        while (!queue2.empty()) {
            line = queue2.front();
            queue2.pop();
            string macCode = MyParser.parseInst(line);  // translate into machine code
            cout << line << "\t->\t" << macCode << endl;
            fOut << macCode << endl;
        }
        cout << "second pass finished" << endl;
        fIn.close();
        fOut.close();
        cout << "file closed" << endl;
    }
    return 0;
}
