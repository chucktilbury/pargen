%debug
%defines
%locations

%{

#include <stdio.h>
#include <stdint.h>

#include "ast.h"
#include "scan.h"
#include "str.h"
#include "str_lst.h"

int errors = 0;


extern StrLst* terms;
extern StrLst* nterms;

AstNode* root_node = NULL;

#ifdef USE_PARSE_TRACE
#   define PARSE_TRACE(fmt, ...) do { \
            printf(">>> %d:%d: ", get_line_no(), get_col_no()); \
            printf((fmt), ## __VA_ARGS__); \
            fputc('\n', stdout); \
        } while(0)
#else
#   define PARSE_TRACE(fmt, ...)
#endif

%}

%union {
    String* str;
    AstNode* node;
};

%token<str> TERMINAL
%token<str> NON_TERMINAL

%type <node> grammar
%type <node> rule
%type <node> production_list
%type <node> production
%type <node> prod_elem
%type <node> zero_or_more
%type <node> one_or_more
%type <node> one_or_zero
%type <node> group

%define parse.error verbose
%locations

%%


grammar
    : rule {
            PARSE_TRACE("first grammar->rule");
            $$ = root_node = create_ast_node(AST_GRAMMAR);
            ((ast_grammar_t*)$$)->list = create_ptr_lst();
            append_ptr_lst(((ast_grammar_t*)$$)->list, (void*)$1);
        }
    | grammar rule {
            PARSE_TRACE("add grammar->rule");
            append_ptr_lst(((ast_grammar_t*)$1)->list, (void*)$2);
        }
    ;

rule
    : NON_TERMINAL ':' production_list ';' {
            PARSE_TRACE("create rule: %s", raw_string($1));
            append_str_lst(nterms, $1);
            $$ = create_ast_node(AST_RULE);
            ((ast_rule_t*)$$)->name = $1;
            ((ast_rule_t*)$$)->list = (ast_production_list_t*)$3;
        }
    ;

production_list
    : production {
            PARSE_TRACE("first rule_list->production");
            $$ = create_ast_node(AST_PRODUCTION_LIST);
            ((ast_production_list_t*)$$)->list = create_ptr_lst();
            append_ptr_lst(((ast_production_list_t*)$$)->list, (void*)$1);
        }
    | production_list '|' production {
            PARSE_TRACE("add rule_list->production");
            append_ptr_lst(((ast_production_list_t*)$1)->list, (void*)$3);
        }
    ;

production
    : prod_elem {
            PARSE_TRACE("first production->prod_elem");
            $$ = create_ast_node(AST_PRODUCTION);
            ((ast_production_t*)$$)->list = create_ptr_lst();
            append_ptr_lst(((ast_production_t*)$$)->list, (void*)$1);
        }
    | production prod_elem {
            PARSE_TRACE("add production->prod_elem");
            append_ptr_lst(((ast_production_t*)$1)->list, (void*)$2);
        }
    ;

prod_elem
    : TERMINAL {
            // glean the definitions of terminal symbols.
            PARSE_TRACE("TERMINAL: %s", raw_string($1));
            const char* str = raw_string($1);
            $$ = create_ast_node(AST_PROD_ELEM);
            ast_terminal_t* node = (ast_terminal_t*)create_ast_node(AST_TERMINAL);
            node->tok = $1;
            ((ast_prod_elem_t*)$$)->node = (AstNode*)node;

            if(search_str_lst(terms, &str[1]) == -1)
                append_str_lst(terms, create_string(&str[1]));

        }
    | NON_TERMINAL {
            // this is a reference to the non-terminal.
            PARSE_TRACE("NON_TERMINAL: %s", raw_string($1));
            $$ = create_ast_node(AST_PROD_ELEM);
            ast_nterm_reference_t* node =
                (ast_nterm_reference_t*)create_ast_node(AST_NTERM_REFERENCE);
            node->tok = $1;
            ((ast_prod_elem_t*)$$)->node = (AstNode*)node;
        }
    | zero_or_more {
            PARSE_TRACE("prod_elem->zero_or_more");
            $$ = create_ast_node(AST_PROD_ELEM);
            ((ast_prod_elem_t*)$$)->node = $1;
        }
    | one_or_more {
            PARSE_TRACE("prod_elem->one_or_more");
            $$ = create_ast_node(AST_PROD_ELEM);
            ((ast_prod_elem_t*)$$)->node = $1;
        }
    | one_or_zero {
            PARSE_TRACE("prod_elem->one_or_zero");
            $$ = create_ast_node(AST_PROD_ELEM);
            ((ast_prod_elem_t*)$$)->node = $1;
        }
    | group {
            PARSE_TRACE("prod_elem->group");
            $$ = create_ast_node(AST_PROD_ELEM);
            ((ast_prod_elem_t*)$$)->node = $1;
        }
    ;

zero_or_more
    : group '*' {
            PARSE_TRACE("zero_or_more->group");
            $$ = create_ast_node(AST_ZERO_OR_MORE);
            ((ast_zero_or_more_t*)$$)->group = (ast_group_t*)$1;
        }
    ;

one_or_more
    : group '+' {
            PARSE_TRACE("one_or_more->group");
            $$ = create_ast_node(AST_ONE_OR_MORE);
            ((ast_one_or_more_t*)$$)->group = (ast_group_t*)$1;
        }
    ;

one_or_zero
    : group '?' {
            PARSE_TRACE("one_or_zero->group");
            $$ = create_ast_node(AST_ZERO_OR_ONE);
            ((ast_zero_or_one_t*)$$)->group = (ast_group_t*)$1;
        }
    ;

group
    : '(' production ')' {
            PARSE_TRACE("group");
            $$ = create_ast_node(AST_GROUP);
            ((ast_group_t*)$$)->prod = (ast_production_t*)$2;
        }
    ;

%%

extern int errors;
void yyerror(const char* s) {

    fprintf(stderr, "%s:%d:%d %s\n",
            get_file_name(), get_line_no(), get_col_no(), s);
    errors++;
}

const char* tokenToStr(int tok) {

    return yysymbol_name(YYTRANSLATE(tok));
}
