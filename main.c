/*
 *
 */
#include <stdio.h>

#include "ast.h"
#include "hash.h"
#include "scan.h"
#include "str.h"
#include "str_lst.h"

extern int yydebug;

StrLst* terms  = NULL;
StrLst* nterms = NULL;

void dump_str_lst(StrLst* lst, const char* str) {

    int post = 0;
    String* ptr;

    printf("%s\n", str);
    post = 0;

    while(NULL != (ptr = iterate_str_lst(lst, &post)))
        printf("%3d. %s\n", post, raw_string(ptr));
}

int main(void) {

    terms  = create_str_lst();
    nterms = create_str_lst();

    yydebug = 0;

    open_file("simple-grammar.txt");
    yyparse();

    sort_str_lst(nterms);
    sort_str_lst(terms);

    // dump_str_lst(terms, "\nTERMINALS");
    // dump_str_lst(nterms, "\nNON TERMINALS");

    traverse_ast(NULL, NULL);

    return 0;
}
