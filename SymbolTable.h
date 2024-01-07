#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdint.h>    
#include <stdlib.h>    
#include <string.h>    

typedef struct {
    char* key;
    uint16_t value;
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    int size;
    int capacity;
    int variableCounter;
    int labelCounter;
} SymbolTable;

SymbolTable* createSymbolTable();
void destroySymbolTable(SymbolTable* symbolTable);
int incLabelCounter(SymbolTable* symbolTable);
void addLabel(SymbolTable* symbolTable, const char* label);
void addVariable(SymbolTable* symbolTable, const char* variable);
uint16_t lookupSymbol(SymbolTable* symbolTable, const char* variable);

#endif


