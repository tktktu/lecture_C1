#ifndef INCLUDE_DATASETS
#define INCLUDE_DATASETS

#define MAX_WORD 20
#define MAX_SENTENCE 1000

#define DICTIONARY_SIZE 20
#define GRAMMAR_SIZE 20

typedef struct DataSet {
    char from[MAX_WORD];
    char to[MAX_WORD];
} DS;

typedef struct DS_ARRAY{
    int size;
    DS *data;
} DSArray;

typedef struct CELL {
    char phrase1[MAX_WORD];
    int x1, y1;
    char phrase2[MAX_WORD];
    struct CELL *p2;
    char phrase3[MAX_WORD];
    struct CELL *p3;
} Cell;

typedef struct CELL_ARRAY {
    Cell *col;
} CArray;

typedef struct CELL_TABLE {
    CArray *row;
    int colSize;
    int rowSize;
} CTable;

typedef struct CYK_RESULT{
    int tableSize;
    int wordNum;
    CTable *tables;
} CYK_Result;

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"C1_util.h"
#include"cyk_C1.h"
#include"sub_cyk_C1.h"
#endif