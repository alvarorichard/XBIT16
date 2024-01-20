#ifndef PARSER_HPP
#define PARSER_HPP

#include <bitset>     // number to binary sting
#include <cstdint>    // this contains uint16_t
#include <map>        // indexable dictionary
#include <stdexcept>  // throw exception
#include <string>     // process c++ string

#include "SymbolTable.hpp"
using namespace std;

extern SymbolTable MyST;  // global symbol table defined in main file

/*The main function of the parser is to break each assembly command into its
underlying components (fields and symbols).*/
class Parser {
   private:
    /* ASM to instruction dictionaries (Code module) */
    map<string, string> compDict;
    map<string, string> destDict;
    map<string, string> jumpDict;
    string prefixA = "0";
    string prefixC = "111";

    void compEntry(string key, string val);
    void destEntry(string key, string val);
    void jumpEntry(string key, string val);
    string compLookup(string instComp);
    string destLookup(string instDest);
    string jumpLookup(string instJump);
    string parseAInst(string inst);
    string parseCInst(string inst);

   public:
    Parser();
    ~Parser();
    string parseInst(string inst);
};

#endif  // PARSER_HPP
