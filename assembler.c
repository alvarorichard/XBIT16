#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#include "Parser.h"
#include "SymbolTable.h"


SymbolTable MyST;
Parser MyParser;

int main(int argc, char const *argv[]) {
    char fnameIn[100], fnameOut[100];
    FILE *fIn, *fOut;
    char line[1000];
    char lineRmComm[1000];
    char *str_iter;
    char macCode[1000];

    if (argc < 2) {
        printf("main(): please specify input file MyAssembler *.asm [*.hack]\n");
        exit(1);
    } else {
        strcpy(fnameIn, argv[1]);

        if (strstr(fnameIn, ".asm") == NULL) {
            printf("main(): input file must be .asm\n");
            exit(1);
        }
        fIn = fopen(fnameIn, "r");

        if (fIn == NULL) {
            printf("main(): unable to open file %s\n", fnameIn);
            exit(1);
        }
        if (argc == 2) {
            int idx = strlen(fnameIn) - 4;
            strncpy(fnameOut, fnameIn, idx);
            fnameOut[idx] = '\0';
            strcat(fnameOut, ".hack");
        } else if (argc == 3) {
            strcpy(fnameOut, argv[2]);
        } else {
            printf("main(): too many parameters\n");
            exit(1);
        }
        fOut = fopen(fnameOut, "w");
        if (fOut == NULL) {
            printf("main(): unable to open file %s\n", fnameOut);
            exit(1);
        }
        printf("output to %s\n", fnameOut);

        printf("\t======\tPre-pass\t=====\t\n");

        while (fgets(line, sizeof(line), fIn)) {
            printf("%s", line);

            char *idx = strstr(line, "//");
            if (idx != NULL) {
                *idx = '\0';
            }
            strcpy(lineRmComm, line);
            str_iter = strchr(lineRmComm, ' ');
            while (str_iter != NULL) {
                strcpy(str_iter, str_iter + 1);
                str_iter = strchr(lineRmComm, ' ');
            }
            if (strlen(lineRmComm) == 0) continue;

            fprintf(fOut, "%s", lineRmComm);
        }

        printf("\t======\tFirst-pass\t======\t\n");
        rewind(fOut);

        while (fgets(line, sizeof(line), fOut)) {
            printf("%s", line);

            char *idx_L = strchr(line, '(');
            char *idx_R = strchr(line, ')');
            if (idx_L != NULL && idx_R != NULL) {
                *idx_R = '\0';
                strcpy(line, idx_L + 1);
                MyST.addLabel(line);
                continue;
            } else {
                MyST.incLabelCounter();
            }
        }

        printf("\t======\tSecond-pass\t======\t\n");
        rewind(fOut);

        while (fgets(line, sizeof(line), fOut)) {
            printf("%s", line);

            strcpy(macCode, MyParser.parseInst(line));
            printf("%s\t->\t%s\n", line, macCode);
            fprintf(fOut, "%s\n", macCode);
        }
        printf("second pass finished\n");
        fclose(fIn);
        fclose(fOut);
        printf("file closed\n");
    }
    return 0;
}


