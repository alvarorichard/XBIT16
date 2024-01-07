#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char key[10];
    char val[10];
} Entry;

typedef struct {
    Entry* entries;
    int size;
} Dictionary;

void dictInit(Dictionary* dict) {
    dict->entries = NULL;
    dict->size = 0;
}

void dictClear(Dictionary* dict) {
    free(dict->entries);
    dict->entries = NULL;
    dict->size = 0;
}

void dictEntry(Dictionary* dict, const char* key, const char* val) {
    dict->entries = realloc(dict->entries, (dict->size + 1) * sizeof(Entry));
    strcpy(dict->entries[dict->size].key, key);
    strcpy(dict->entries[dict->size].val, val);
    dict->size++;
}

const char* dictLookup(Dictionary* dict, const char* key) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            return dict->entries[i].val;
        }
    }
    return dict->entries[0].val;
}

typedef struct {
    Dictionary compDict;
    Dictionary destDict;
    Dictionary jumpDict;
} Parser;

void parserInit(Parser* parser) {
    dictInit(&parser->compDict);
    dictInit(&parser->destDict);
    dictInit(&parser->jumpDict);
    
    dictEntry(&parser->compDict, "0", "0101010");
    dictEntry(&parser->compDict, "1", "0111111");
    dictEntry(&parser->compDict, "-1", "0111010");
    dictEntry(&parser->compDict, "D", "0001100");
    dictEntry(&parser->compDict, "A", "0110000");
    dictEntry(&parser->compDict, "!D", "0001101");
    dictEntry(&parser->compDict, "!A", "0110001");
    dictEntry(&parser->compDict, "-D", "0001111");
    dictEntry(&parser->compDict, "-A", "0110011");
    dictEntry(&parser->compDict, "D+1", "0011111");
    dictEntry(&parser->compDict, "1+D", "0011111");
    dictEntry(&parser->compDict, "A+1", "0110111");
    dictEntry(&parser->compDict, "1+A", "0110111");
    dictEntry(&parser->compDict, "D-1", "0001110");
    dictEntry(&parser->compDict, "A-1", "0110010");
    dictEntry(&parser->compDict, "D+A", "0000010");
    dictEntry(&parser->compDict, "A+D", "0000010");
    dictEntry(&parser->compDict, "D-A", "0010011");
    dictEntry(&parser->compDict, "A-D", "0000111");
    dictEntry(&parser->compDict, "D&A", "0000000");
    dictEntry(&parser->compDict, "A&D", "0000000");
    dictEntry(&parser->compDict, "D|A", "0010101");
    dictEntry(&parser->compDict, "A|D", "0010101");
    dictEntry(&parser->compDict, "M", "1110000");
    dictEntry(&parser->compDict, "!M", "1110001");
    dictEntry(&parser->compDict, "-M", "1110011");
    dictEntry(&parser->compDict, "M+1", "1110111");
    dictEntry(&parser->compDict, "1+M", "1110111");
    dictEntry(&parser->compDict, "M-1", "1110010");
    dictEntry(&parser->compDict, "D+M", "1000010");
    dictEntry(&parser->compDict, "M+D", "1000010");
    dictEntry(&parser->compDict, "D-M", "1010011");
    dictEntry(&parser->compDict, "M-D", "1000111");
    dictEntry(&parser->compDict, "D&M", "1000000");
    dictEntry(&parser->compDict, "M&D", "1000000");
    dictEntry(&parser->compDict, "D|M", "1010101");
    dictEntry(&parser->compDict, "M|D", "1010101");
    
    dictEntry(&parser->destDict, "null", "000");
    dictEntry(&parser->destDict, "M", "001");
    dictEntry(&parser->destDict, "D", "010");
    dictEntry(&parser->destDict, "MD", "011");
    dictEntry(&parser->destDict, "DM", "011");
    dictEntry(&parser->destDict, "A", "100");
    dictEntry(&parser->destDict, "AM", "101");
    dictEntry(&parser->destDict, "MA", "101");
    dictEntry(&parser->destDict, "AD", "110");
    dictEntry(&parser->destDict, "DA", "110");
    dictEntry(&parser->destDict, "AMD", "111");
    dictEntry(&parser->destDict, "ADM", "111");
    dictEntry(&parser->destDict, "MAD", "111");
    dictEntry(&parser->destDict, "MDA", "111");
    dictEntry(&parser->destDict, "DAM", "111");
    dictEntry(&parser->destDict, "DMA", "111");
    
    dictEntry(&parser->jumpDict, "null", "000");
    dictEntry(&parser->jumpDict, "JGT", "001");
    dictEntry(&parser->jumpDict, "JEQ", "010");
    dictEntry(&parser->jumpDict, "JGE", "011");
    dictEntry(&parser->jumpDict, "JLT", "100");
    dictEntry(&parser->jumpDict, "JNE", "101");
    dictEntry(&parser->jumpDict, "JLE", "110");
    dictEntry(&parser->jumpDict, "JMP", "111");
}

void parserClear(Parser* parser) {
    dictClear(&parser->compDict);
    dictClear(&parser->destDict);
    dictClear(&parser->jumpDict);
}

const char* parserCompLookup(Parser* parser, const char* instComp) {
    return dictLookup(&parser->compDict, instComp);
}

const char* parserDestLookup(Parser* parser, const char* instDest) {
    return dictLookup(&parser->destDict, instDest);
}

const char* parserJumpLookup(Parser* parser, const char* instJump) {
    return dictLookup(&parser->jumpDict, instJump);
}

const char* parserParseInst(Parser* parser, const char* inst) {
    char tempInst[100];
    
    if (strstr(inst, "@") != NULL) {
        strncpy(tempInst, inst + 1, strlen(inst) - 1);
        tempInst[strlen(inst) - 1] = '\0';
        return parserParseAInst(parser, tempInst);
    } else {
        return parserParseCInst(parser, inst);
    }
}

const char* parserParseAInst(Parser* parser, const char* inst) {
    if (strspn(inst, "0123456789") == strlen(inst)) {
        char result[17];
        sprintf(result, "0%s", inst);
        return strdup(result);
    } else {
        if (strcspn(inst, "0123456789") < strspn(inst, "0123456789")) {
            fprintf(stderr, "Parser::parseAInst(): invalid A-inst: number first\n");
            exit(1);
        }
        uint16_t addr = lookupSymbol(inst);
        char result[17];
        sprintf(result, "0%u", addr);
        return strdup(result);
    }
}

const char* parserParseCInst(Parser* parser, const char* inst) {
    char* idxe = strchr(inst, '=');
    char* idxsc = strchr(inst, ';');
    char instDest[10];
    char instComp[10];
    char instJump[10];
    
    if (idxe == NULL) {
        strcpy(instDest, "null");
    } else {
        strncpy(instDest, inst, idxe - inst);
        instDest[idxe - inst] = '\0';
    }
    
    if (idxsc == NULL) {
        strcpy(instJump, "null");
    } else {
        strcpy(instJump, idxsc + 1);
    }
    
    strncpy(instComp, idxe + 1, idxsc - idxe - 1);
    instComp[idxsc - idxe - 1] = '\0';
    
    char result[18];
    sprintf(result, "111%s%s%s", parserCompLookup(parser, instComp), parserDestLookup(parser, instDest), parserJumpLookup(parser, instJump));
    return strdup(result);
}

// int main() {
//     Parser parser;
//     parserInit(&parser);
    
//     // Example usage
//     const char* inst = "D=A+1;JMP";
//     const char* result = parserParseInst(&parser, inst);
//     printf("%s\n", result);
    
//     parserClear(&parser);
//     return 0;
// }


