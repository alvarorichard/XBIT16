#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>    
#include <stdlib.h>    
#include <stdio.h>     
#include <string.h>    
#include "SymbolTable.h"
extern SymbolTable MyST;  

typedef struct {
    char key[10];
    char val[10];
} Entry;

typedef struct {
    Entry entries[10];
    int count;
} Map;

typedef struct {
    char prefixA[2];
    char prefixC[4];
    Map compDict;
    Map destDict;
    Map jumpDict;
} Parser;

void compEntry(Map* map, const char* key, const char* val);
void destEntry(Map* map, const char* key, const char* val);
void jumpEntry(Map* map, const char* key, const char* val);
const char* compLookup(const Map* map, const char* instComp);
const char* destLookup(const Map* map, const char* instDest);
const char* jumpLookup(const Map* map, const char* instJump);
const char* parseAInst(const Parser* parser, const char* inst);
const char* parseCInst(const Parser* parser, const char* inst);

Parser* createParser();
void destroyParser(Parser* parser);
const char* parseInst(const Parser* parser, const char* inst);

#endif


