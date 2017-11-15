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

void combination(CYK_Result result, int i, int j, const DSArray gram) {
    //Result.row[i].col[j] = current cell
    //Result.row[i].col[i + k] = left cell
    //Result.row[i + k + 1].col[j] = under cell
    int z = 0;//index of current cell table
    for (int k = 0; k < j - i; k++) {
        //Skip no part of speech cells
        if (strcmp(result.tables[0].row[i].col[i + k].phrase1, "") != 0 &&
            strcmp(result.tables[0].row[i + k + 1].col[j].phrase1, "") != 0) {
            Cell tmp = {"", i, j, "", NULL, "", NULL};
            int x = 0;//index of left cell tables
            do {
                strcpy(tmp.phrase2, result.tables[x].row[i].col[i + k].phrase1);
                int y = 0;//index of under cell tables
                do {
                    strcpy(tmp.phrase3, result.tables[y].row[i + k + 1].col[j].phrase1);
                    tmp.p2 = &result.tables[x].row[i].col[i + k];
                    tmp.p3 = &result.tables[y].row[i + k + 1].col[j];
                    if (getPhrase(tmp.phrase1, tmp.phrase2, tmp.phrase3, gram) != -1) {
                        do {
                            result.tables[z++].row[i].col[j] = tmp;
                        //If there are other phrase for one combination, repeat loop
                        } while (getPhrase(tmp.phrase1, NULL, NULL, gram) != -1);
                    }
                //If there are under cell's data on other tables, repeat loop
                } while (strcmp(result.tables[++y].row[i + k + 1].col[j].phrase1, "") != 0);
            //If there are left cell's data on other tables, repeat loop
            } while (strcmp(result.tables[++x].row[i].col[i + k].phrase1, "") != 0);
        }
    }
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

int analyze_cyk(CYK_Result result, char *target, const DSArray dict, const DSArray gram) {
    /*---------------Split sentence and get part of speech-------------*/
    for (int i = 0; i < result.wordNum; i++) {
        Cell tmp = {"", i, i, "", NULL, "", NULL};
        int index = 0;//Index of tables
        //Split sentence and Memorize one word
        target = split(tmp.phrase2, target, " .", "\n");
        //Search part of speech by dictionary
        if (getPart(tmp.phrase1, tmp.phrase2, dict))
            printf("error:%s\n", tmp.phrase2);
        //Substitute result's cell for tmp
        result.tables[index++].row[i].col[i] = tmp;
        //Search a phrase composed of only one part of speech
        if (getPhrase(tmp.phrase1, tmp.phrase1, "", gram) != -1) {
            do {
                result.tables[index++].row[i].col[i] = tmp;
            } while (getPhrase(tmp.phrase1, NULL, NULL, gram) != -1);
        }
        target++;
    }
    /*------------------------------end--------------------------------*/
//    for (int i = 0; i < result.wordNum; i++)
//        printTable(result.tables[i]);//debug
    /*---------------------Search connection of phrases------------------------*/
    for (int i = 1; i < result.tables[0].rowSize; i++) {
        for (int j = 0; j + i < result.tables[0].colSize; j++) {
            combination(result, j, i + j, gram);
        }
    }
    /*------------------------------end----------------------------------------*/
    for (int i = 0; i < result.wordNum; i++)
        printTable(result.tables[i]);//debug
    /*---------------------Print S formula------------------------*/
    for (int i = 0; i < result.wordNum; i++) {
        if (printSFormula(&result.tables[i].row[0].col[result.wordNum - 1]) != 1)
            puts("");
    }
    /*-----------------------------end---------------------------*/
    return 0;
}