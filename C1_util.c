#include "parameter.h"

int readCsv(const char *filename, DS *args) {
    int i;
    FILE *fp;
    char str[MAX_WORD] = {'\0'};
    //File open
    if ((fp = fopen(filename, "r")) == NULL) {
        return -1;
    }
    //Read a CSV file
    i = 0;
    while (fgets(str, MAX_WORD, fp) != NULL) {
        strcpy((args + i)->from, strtok(str, ",\n"));
        strcpy((args + i)->to, strtok(NULL, ",\n"));
        i++;
    }
    //File close
    fclose(fp);
    return i;
}

char *split(char *str, char *original, const char *delimiter, const char *skipchar) {
    if (original == NULL || *original == '\0')
        return NULL;
    //If contained *original in 'delimiter', stop a loop
    while (!strchr(delimiter, *original)) {
        //If contained *original in 'skipchar', skip the character in original
        while (strchr(skipchar, *original) && *original != '\0')
            original++;
        //Copy a character from *original to *str
        *str = *original;
        //If found end of sentence, return NULL
        if (*original == '\0') {
            return original;
        }
        str++;
        original++;
    }
    *str = '\0';
    return original;
}

//debug
void showAll(const DS datasets[], int n) {
    int i = 0;
    for (i = 0; i < n; i++)
        printf("%s\t", datasets[i].from);
    puts("");
    for (i = 0; i < n; i++)
        printf("%s\t", datasets[i].to);
    puts("");
}

void cell_table_construct(CTable *tables, const Cell cell_init) {
    tables->row = (CArray *) malloc(sizeof(CArray) * tables->rowSize);
    for (int i = 0; i < tables->rowSize; i++) {
        tables->row[i].col = (Cell *) malloc(sizeof(Cell) * tables->colSize);
        for (int j = 0; j < tables->colSize; j++) {
            tables->row[i].col[j] = cell_init;
        }
    }
}

void cell_table_destruct(CTable *tables) {
    for (int i = 0; i < tables->rowSize; i++) {
        free(tables->row[i].col);
    }
    free(tables->row);
}