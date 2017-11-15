#include "parameter.h"

int readCsv(const char *, DS *);
char *split(char *,char *, const char *, const char *);
void showAll(const DS *, int);

void cell_table_construct(CTable *, const Cell);
void cell_table_destruct(CTable *);

