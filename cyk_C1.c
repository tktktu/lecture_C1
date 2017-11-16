#include "parameter.h"

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