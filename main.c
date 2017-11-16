#include "parameter.h"

#define count_spaces(cnt, target_sentence) \
for (int chara_cnt = 0; *((target_sentence) + chara_cnt) != '\0'; chara_cnt++){\
    if (*((target_sentence) + chara_cnt) == ' '){\
        (cnt)++;\
    }\
}

int main(void) {
    char grammar_file[MAX_WORD] = "";
    char dictionary_file[MAX_WORD] = "";
    char sentence[MAX_SENTENCE] = "";
    /*-----------------------------Define dictionary and grammar size----------------------*/
    DSArray dict = {DICTIONARY_SIZE, NULL};
    DSArray gram = {GRAMMAR_SIZE, NULL};

    dict.data = (DS *) malloc(sizeof(DS) * dict.size);
    gram.data = (DS *) malloc(sizeof(DS) * gram.size);
    /*-----------------------------end----------------------------------------*/
    /*---------------------Read a dictionary---------------------------*/
    puts("DictionaryFile");
    printf(">>>");
    fgets(dictionary_file, MAX_WORD, stdin);
    //Delete '\n'
    dictionary_file[strlen(dictionary_file) - 1] = '\0';
    if (readCsv(dictionary_file, dict.data) == -1) {
        printf("Error NotFoundFile : %s\n", dictionary_file);
        return 0;
    }
    /*------------------------------end--------------------------------*/
    /*----------------------Read a grammar----------------------------*/
    puts("GrammarFile");
    printf(">>>");
    fgets(grammar_file, MAX_WORD, stdin);
    //Delete '\n'
    grammar_file[strlen(grammar_file) - 1] = '\0';
    if (readCsv(grammar_file, gram.data) == -1) {
        printf("Error NotFoundFile : %s\n", grammar_file);
        return 0;
    }
    /*---------------------------end---------------------------------*/
    /*--------------------Read a target sentence----------------------*/
    puts("InputSentence");
    printf(">>>");
    fgets(sentence, MAX_SENTENCE, stdin);
    if (strlen(sentence) >= MAX_SENTENCE) {
        puts("Error:LongSentence");
        return 0;
    }
    /*------------------------------end-------------------------------*/
    /*-----------------------Initilize Cells---------------------------*/
    int word = 1;
    count_spaces(word, sentence);
    //Number of table = GRAMMARSIZE
    CYK_Result result = {GRAMMAR_SIZE, word, NULL};
    Cell init_cell = {"", -1, -1, "", NULL, "", NULL};
    result.tables = (CTable *)malloc(sizeof(CTable) * result.tableSize);
    for (int i = 0; i < result.tableSize; i++) {
        result.tables[i].rowSize = result.tables[i].colSize = word;
        cell_table_construct(&result.tables[i], init_cell);
    }
    /*----------------------------end----------------------------------*/
    /*-----------------------Analyze---------------------------*/
    if(analyze_cyk(result, sentence, dict, gram) != 0){
        printf("NotSentence:%s\n", sentence);
    }
    /*-------------------------end-----------------------------*/
    /*-----------------------Memory free--------------------------*/
    for (int i = 0; i < result.tableSize; i++)
        cell_table_destruct(&result.tables[i]);
    free(result.tables);
    free(dict.data);
    free(gram.data);
    /*---------------------------end------------------------------*/
    return 0;
}