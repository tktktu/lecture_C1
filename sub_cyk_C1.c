#include "parameter.h"
#define printSpaces(n) \
for(int number_of_space = 0; number_of_space < (n); number_of_space++)\
    printf(" ");\

int getPart(char *part, const char *target, const DSArray dict) {
    int i;
    //If there is a target word in dictionary, Copy a part of speech
    for (i = 0; i < dict.size; i++) {
        if (!strcmp(dict.data[i].from, target)) {
            strcpy(part, dict.data[i].to);
            return 0;
        }
    }
    return 1;
}

int getPhrase(char *phrase1, const char *phrase2, const char *phrase3, const DSArray gram) {
    static int i = 0;
    static char subPhrase[MAX_WORD];
    if (phrase2 != NULL && phrase3 != NULL) {
        i = 0;
        sprintf(subPhrase, "%s+%s", phrase2, phrase3);
    }
    while (i < gram.size) {
        if (!strcmp(gram.data[i].to, subPhrase)) {
            strcpy(phrase1, gram.data[i].from);
            return i++;
        }
        i++;
    }
    return -1;
}
void printTable(CTable result) {
    for (int i = 0; i < result.rowSize; i++) {
        printSpaces(16 * i);
        for (int j = i; j < result.colSize; j++) {
            char *p1 = result.row[i].col[j].phrase1;
            char *p2 = result.row[i].col[j].phrase2;
            char *p3 = result.row[i].col[j].phrase3;
            printf("%4s(%4s,%4s)|", p1, p2, p3);
        }
        puts("");
    }
}

int printSFormula(Cell *result) {
    if (!strcmp(result->phrase1, ""))return 1;
    printf("(");
    printf("%s%d%d", result->phrase1, result->x1, result->y1);
    if (result->p2 != NULL) printSFormula(result->p2); else printf(",\"%s\"", result->phrase2);
    if (result->p3 != NULL) printSFormula(result->p3);
    printf(")");
    return 0;
}
