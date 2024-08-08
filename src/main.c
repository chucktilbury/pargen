/*
 *
 */
#include <stdio.h>

#include "ast.h"
#include "emit.h"
#include "cmdline.h"
#include "hash.h"
#include "scan.h"
#include "str.h"
#include "str_lst.h"

extern int yydebug;

StrLst* terms  = NULL;
StrLst* nterms = NULL;

void init(int argc, char** argv) {

    init_cmdline("Simple Parser Generator", "", "Parser Generator", "0.0.0");

    // set the optional file names
    add_cmdline('a', "ast", "ast_name", "name of the ast files, possibly a full path",
                "_ast", NULL, CMD_STR|CMD_RARG);
    add_cmdline('p', "parser", "parse_name", "name of the parser files, possibly a full path",
                "_parser", NULL, CMD_STR|CMD_RARG);

    // verbosity level, values 0-10
    add_cmdline('v', "verbosity", "verbo", "control how much text is displayed during execution",
                "0", NULL, CMD_NUM | CMD_RARG);

    // standard options that control the command line parser behaviors
    add_cmdline('V', "version", NULL, "show the version", NULL, show_version, CMD_NARG);
    add_cmdline('h', "help", NULL, "show this help text", NULL, show_help, CMD_NARG);

    // list of files should always be the last one
    add_cmdline(0, NULL, "list of files", "list of files to be processed", NULL,
                NULL, CMD_STR | CMD_REQD);

    // see what we got.
    parse_cmdline(argc, argv, 0);
}

void dump_str_lst(StrLst* lst, const char* str) {

    int post = 0;
    String* ptr;

    printf("%s\n", str);
    post = 0;

    while(NULL != (ptr = iterate_str_lst(lst, &post)))
        printf("%3d. %s\n", post, raw_string(ptr));
}

#include "regurg.h"
int main(int argc, char** argv) {

    init(argc, argv);
    terms  = create_str_lst();
    nterms = create_str_lst();

    yydebug = 0;

    open_file(get_cmdline("list of files"));
    yyparse();

    sort_str_lst(nterms);
    sort_str_lst(terms);

    // dump_str_lst(terms, "\nTERMINALS");
    // dump_str_lst(nterms, "\nNON TERMINALS");

    // traverse_ast(NULL, NULL);
    // regurg();

    emit();

    return 0;
}
