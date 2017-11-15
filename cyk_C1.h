#include "parameter.h"
int getPart(char *, const char *,const DSArray);
int getPhrase(char *, const char *, const char *, const DSArray);
void printTable(CTable );
int printSFormula(Cell *result);
void combination(CYK_Result , int , int , const DSArray);
int analyze_cyk(CYK_Result , char *, const DSArray, const DSArray);
