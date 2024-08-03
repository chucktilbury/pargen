/*
 *
 */
#include <stdio.h>

#include "scan.h"
#include "hash.h"
#include "str.h"
#include "str_lst.h"
#include "ast.h"

extern int yydebug;

StrLst* terms = NULL;
StrLst* nterms = NULL;

int main(void) {

    terms = create_str_lst();
    nterms = create_str_lst();

    yydebug = 0;

    open_file("simple-grammar.txt");
    yyparse();

    /*
    int post;
    String* str;
    printf("\nTERMINALS\n");
    post = 0;
    while(NULL != (str = iterate_str_lst(terms, &post)))
        printf("%3d. %s\n", post+1, raw_string(str));

    printf("\nNON TERMINALS\n");
    post = 0;
    while(NULL != (str = iterate_str_lst(nterms, &post)))
        printf("%3d. %s\n", post+1, raw_string(str));
    */

    traverse_ast(NULL, NULL);

    return 0;
}
