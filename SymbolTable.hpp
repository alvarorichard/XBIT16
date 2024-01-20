#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <cstdint>    // this contains uint16_t
#include <map>        // indexable dictionary
#include <stdexcept>  // throw exception
#include <string>     // process c++ string
using namespace std;
class SymbolTable {
   private:
    /* the symble table storage space */
    map<string, uint16_t> table;
    int VariableCounter;
    int LabelCounter;

   public:
    SymbolTable();                      // initialize symbol table, set start variabl counter
    ~SymbolTable();                     // free up all used memory
    int incLabelCounter();              // tracking the label address
    void addLabel(string Label);        // add new label to the table
    void addVariable(string Var);       // add new variabl to the table
    uint16_t lookupSymbol(string Var);  // get memory address of a give symbol
};

#endif  // SYMBOLTABLE_HPP
