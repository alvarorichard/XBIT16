#include "SymbolTable.hpp"
SymbolTable::SymbolTable() {
    table.clear();

    /* Initialize predefined symbles */
    table.insert(pair<string, uint16_t>("SP", 0));
    table.insert(pair<string, uint16_t>("LCL", 1));
    table.insert(pair<string, uint16_t>("ARG", 2));
    table.insert(pair<string, uint16_t>("THIS", 3));
    table.insert(pair<string, uint16_t>("THAT", 4));
    /* Virtual register R0 - R15 */
    table.insert(pair<string, uint16_t>("R0", 0));
    table.insert(pair<string, uint16_t>("R1", 1));
    table.insert(pair<string, uint16_t>("R2", 2));
    table.insert(pair<string, uint16_t>("R3", 3));
    table.insert(pair<string, uint16_t>("R4", 4));
    table.insert(pair<string, uint16_t>("R5", 5));
    table.insert(pair<string, uint16_t>("R6", 6));
    table.insert(pair<string, uint16_t>("R7", 7));
    table.insert(pair<string, uint16_t>("R8", 8));
    table.insert(pair<string, uint16_t>("R9", 9));
    table.insert(pair<string, uint16_t>("R10", 10));
    table.insert(pair<string, uint16_t>("R11", 11));
    table.insert(pair<string, uint16_t>("R12", 12));
    table.insert(pair<string, uint16_t>("R13", 13));
    table.insert(pair<string, uint16_t>("R14", 14));
    table.insert(pair<string, uint16_t>("R15", 15));
    /* MMIO */
    table.insert(pair<string, uint16_t>("SCREEN", 16384));
    table.insert(pair<string, uint16_t>("KBD", 24576));
    /* Variables are mapped to consecutive memory
    locations as they are first encountered, starting
    at RAM address 16  */
    VariableCounter = 16;
}

/* Symbol table destructor */
SymbolTable::~SymbolTable() {
    table.clear();  // delete all content in symble table
}

int SymbolTable::incLabelCounter() {  // increament label counter
    return ++LabelCounter;
}

void SymbolTable::addLabel(string Label) {
    if (table.find(Label) != table.end()) {
        throw runtime_error("addLabel(): redefined label!");
    } else {
        table.insert(pair<string, uint16_t>(Label, LabelCounter));
    }
}

void SymbolTable::addVariable(string Var) {
    if (table.find(Var) != table.end()) {  // skip if variable is already in
        table.insert(pair<string, uint16_t>(Var, VariableCounter));
        VariableCounter++;
    }
}

uint16_t SymbolTable::lookupSymbol(string Var) {
    addVariable(Var);
    return table.find(Var)->second;
}
