#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char* key;
    uint16_t value;
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    int size;
    int capacity;
} SymbolTable;

void initSymbolTable(SymbolTable* table) {
    table->pairs = NULL;
    table->size = 0;
    table->capacity = 0;
}

void clearSymbolTable(SymbolTable* table) {
    for (int i = 0; i < table->size; i++) {
        free(table->pairs[i].key);
    }
    free(table->pairs);
    table->pairs = NULL;
    table->size = 0;
    table->capacity = 0;
}

void insertKeyValuePair(SymbolTable* table, const char* key, uint16_t value) {
    KeyValuePair pair;
    pair.key = strdup(key);
    pair.value = value;

    if (table->size >= table->capacity) {
        table->capacity = (table->capacity == 0) ? 1 : table->capacity * 2;
        table->pairs = realloc(table->pairs, table->capacity * sizeof(KeyValuePair));
    }

    table->pairs[table->size++] = pair;
}

int findKeyValuePair(SymbolTable* table, const char* key) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->pairs[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

int incLabelCounter(int* counter) {
    return ++(*counter);
}

void addLabel(SymbolTable* table, const char* label, int* counter) {
    int index = findKeyValuePair(table, label);
    if (index != -1) {
        fprintf(stderr, "addLabel(): redefined label!\n");
        exit(1);
    } else {
        insertKeyValuePair(table, label, *counter);
    }
}

void addVariable(SymbolTable* table, const char* var, int* counter) {
    int index = findKeyValuePair(table, var);
    if (index == -1) {
        insertKeyValuePair(table, var, *counter);
        (*counter)++;
    }
}

uint16_t lookupSymbol(SymbolTable* table, const char* var, int* counter) {
    addVariable(table, var, counter);
    int index = findKeyValuePair(table, var);
    return table->pairs[index].value;
}

// int main() {
//     SymbolTable table;
//     initSymbolTable(&table);

//     insertKeyValuePair(&table, "SP", 0);
//     insertKeyValuePair(&table, "LCL", 1);
//     insertKeyValuePair(&table, "ARG", 2);
//     insertKeyValuePair(&table, "THIS", 3);
//     insertKeyValuePair(&table, "THAT", 4);
//     insertKeyValuePair(&table, "R0", 0);
//     insertKeyValuePair(&table, "R1", 1);
//     insertKeyValuePair(&table, "R2", 2);
//     insertKeyValuePair(&table, "R3", 3);
//     insertKeyValuePair(&table, "R4", 4);
//     insertKeyValuePair(&table, "R5", 5);
//     insertKeyValuePair(&table, "R6", 6);
//     insertKeyValuePair(&table, "R7", 7);
//     insertKeyValuePair(&table, "R8", 8);
//     insertKeyValuePair(&table, "R9", 9);
//     insertKeyValuePair(&table, "R10", 10);
//     insertKeyValuePair(&table, "R11", 11);
//     insertKeyValuePair(&table, "R12", 12);
//     insertKeyValuePair(&table, "R13", 13);
//     insertKeyValuePair(&table, "R14", 14);
//     insertKeyValuePair(&table, "R15", 15);
//     insertKeyValuePair(&table, "SCREEN", 16384);
//     insertKeyValuePair(&table, "KBD", 24576);

//     int variableCounter = 16;

//     clearSymbolTable(&table);

//     return 0;
// }


