/**
 * @file emit_ast_header.c
 *
 * @brief This is a AST traverse pass that emits the AST source code and the
 * AST header file. This relies on the command line features to retrieve the
 * configuration.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-08-07
 * @copyright Copyright (c) 2024
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "emit.h"
#include "ast.h"
#include "str.h"
#include "str_lst.h"
#include "cmdline.h"

static const char* file_pre[] = {
    " */",
    "#ifndef _EMIT_AST_HEADER_H_",
    "#define _EMIT_AST_HEADER_H_",
    "",
    "#include \"ptr_lst.h\"",
    "#include \"str.h\"",
    "",
    "void traverse_ast(AstPassFunc pre, AstPassFunc post);",
    "AstNode* create_ast_node(AstNodeType type);",
    "",
    "typedef int (*AstPassFunc)(AstNode*);",
    "",
    NULL
};

static const char* ds_pre[] = {
    "/*",
    " * These data structures are determined by the input file syntax.",
    " */",
    "typedef struct _ast_node_ {",
    "    AstNodeType type;",
    "} AstNode;",
    "",
    "typedef struct _ast_terminal_ {",
    "    AstNode type;",
    "    String* tok;",
    "    String* name;",
    "} ast_terminal_t;",
    "",
    "typedef struct _ast_non_terminal_ {",
    "    AstNode type;",
    "    String* tok;",
    "    String* name;",
    "} ast_non_terminal_t;",
    "",
    "typedef struct _ast_zero_or_one_ {",
    "    AstNode type;",
    "    struct _ast_group_* group;",
    "} ast_zero_or_one_t;",
    "",
    "typedef struct _ast_one_or_more_ {",
    "    AstNode type;",
    "    struct _ast_group_* group;",
    "} ast_one_or_more_t;",
    "",
    "typedef struct _ast_zero_or_more_ {",
    "    AstNode type;",
    "    struct _ast_group_* group;",
    "} ast_zero_or_more_t;",
    "",
    "typedef struct _ast_group_ {",
    "    AstNode type;",
    "    // list of one or more ast_prod_elem_t",
    "    struct _ast_production_* prod;",
    "} ast_group_t;",
    "",
    "typedef struct _ast_production_list_ {",
    "    AstNode type;",
    "    // list of productions",
    "    PtrLst* list;",
    "} ast_production_list_t;",
    "",
    "typedef struct _ast_production_ {",
    "    AstNode type;",
    "    // list of ast_prod_elem_t",
    "    PtrLst* list;",
    "} ast_production_t;",
    "",
    "typedef struct _ast_prod_elem_ {",
    "    AstNode type;",
    "    // a single item that could be a repetition function, another",
    "    // non-terminal or a terminal",
    "    AstNode* node;",
    "} ast_prod_elem_t;",
    "",
    "typedef struct _ast_grammar_ {",
    "    AstNode type;",
    "    // List of rules.",
    "    PtrLst* list;",
    "} ast_grammar_t;",
    "",
    "typedef struct _ast_rule_ {",
    "    AstNode type;",
    "    // a rule definition",
    "    String* name;",
    "    struct _ast_production_list_* list;",
    "} ast_rule_t;",
    "",
    "/*",
    " * User defined non-terminals.",
    " */",
    NULL
};

static const char* file_post[] = {
    "",
    "#endif /* _EMIT_AST_HEADER_H_ */",
    "",
    "/*",
    " * End of generated file.",
    " */",
    "",
    NULL
};

static const char* types_pre[] = {
    "typedef enum {",
    "    /*",
    "     * These are a result of the input grammar syntax.",
    "     */",
    "    AST_TERMINAL = 100,",
    "    AST_NON_TERMINAL,",
    "    AST_ZERO_OR_ONE,",
    "    AST_ONE_OR_MORE,",
    "    AST_ZERO_OR_MORE,",
    "    AST_GROUP,",
    "    AST_PRODUCTION_LIST,",
    "    AST_PRODUCTION,",
    "    AST_PROD_ELEM,",
    "    AST_GRAMMAR,",
    "    AST_RULE,",
    "",
    "    /*",
    "     * These are from the user's input grammar.",
    "     */",
    NULL
};

static const char* types_post[] = {
    "} AstNodeType;\n\n",
    NULL
};

static const char* protos[] = {
    "/*",
    " * These protos are defined by the syntax of the input file.",
    " */",
    "void ast_terminal(ast_terminal_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_non_terminal(ast_non_terminal_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_grammar(ast_grammar_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_rule(ast_rule_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_production_list(ast_production_list_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_production(ast_production_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_prod_elem(ast_prod_elem_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_zero_or_one(ast_zero_or_one_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_one_or_more(ast_one_or_more_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_zero_or_more(ast_zero_or_more_t* node, AstPassFunc pre, AstPassFunc post);",
    "void ast_group(ast_group_t* node, AstPassFunc pre, AstPassFunc post);",
    "",
    "/*",
    " * These protos are defined by the input grammar.",
    " */",
    NULL
};

/*
 * These are the lists that were generated when the parser ran.
 */
extern StrLst* terms;
extern StrLst* nterms;

// output file handle for the ast header file that is generated by this module.
static FILE* outfile = NULL;
static int in_rule = 0;
static int in_group = 0;


static int _error(const char* fn, int state) {
    fprintf(stderr, "Fatal internal error: invalid state in %s: %d\n", fn, state);
    abort();
}

static int dummy(void) {
    // do nothing.
    return 0;
}

static int pre_rule(AstNode* node) {

    fprintf(outfile, "typedef struct _ast_%s_ {\n    AstNode type;\n", 
            raw_string(((ast_rule_t*)node)->name));
    in_rule++;

    return 0;
}

static int post_rule(AstNode* node) {

    fprintf(outfile, "} ast_%s_t;\n\n", 
            raw_string(((ast_rule_t*)node)->name));
    in_rule--;

    return 0;
}

static int pre_group() {

    in_group++;
    return 0;
}

static int post_group() {
    
    in_group--;
    return 0;
}

static int pre_nterm(AstNode* node) {

    if(in_rule > 0 && in_group < 2) {
        const char* tok = raw_string(((ast_non_terminal_t*)node)->tok);
        const char* name = raw_string(((ast_non_terminal_t*)node)->name);
        fprintf(outfile, "    struct _ast_%s_* %s;\n", tok, name? name: tok);
    }

    return 0;
}

static int pre_term(AstNode* node) {

    if(in_rule > 0 && in_group < 2) {
        String* str = ((ast_terminal_t*)node)->tok;
        String* tmp = copy_string(str);
        lower_string(tmp);
        const char* tstr = raw_string(tmp);

        if(tstr[0] != 's')
            fprintf(outfile, "    TokenType %s_type;\n", &tstr[4]);
        else
            fprintf(outfile, "    String* %s_str;\n", &tstr[4]);
        destroy_string(tmp);
    }

    return 0;
}

static int ast_ds_pre(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? pre_term(node) :
            (node->type == AST_NON_TERMINAL) ? pre_nterm(node) :
            (node->type == AST_ZERO_OR_ONE)     ? dummy() :
            (node->type == AST_ONE_OR_MORE)     ? dummy() :
            (node->type == AST_ZERO_OR_MORE)    ? dummy() :
            (node->type == AST_GROUP)           ? pre_group() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? pre_rule(node) :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? dummy() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  _error(__func__, node->type);
}

static int ast_ds_post(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? dummy() :
            (node->type == AST_NON_TERMINAL) ? dummy() :
            (node->type == AST_ZERO_OR_ONE)     ? dummy() :
            (node->type == AST_ONE_OR_MORE)     ? dummy() :
            (node->type == AST_ZERO_OR_MORE)    ? dummy() :
            (node->type == AST_GROUP)           ? post_group() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? post_rule(node) :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? dummy() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  _error(__func__, node->type);
}

static void emit_ds(void) {

    emit_block(outfile, ds_pre);
    traverse_ast(ast_ds_pre, ast_ds_post);
}

static void emit_protos(void) {

    int mark = 0;
    String* str;

    emit_block(outfile, protos);

    while(NULL != (str = iterate_str_lst(nterms, &mark))) {
        const char* tpt = raw_string(str);
        fprintf(outfile, "void ast_%s(ast_%s_t* node, AstPassFunc pre, AstPassFunc post);\n",
            tpt, tpt);
    }
}

static void emit_type_list(void) {

    int mark = 0;
    String* str;

    emit_block(outfile, types_pre);

    while(NULL != (str = iterate_str_lst(nterms, &mark))) {
        String* cpy_ptr = copy_string(str);
        upper_string(cpy_ptr);
        fprintf(outfile, "    AST_%s,\n", raw_string(cpy_ptr));
        destroy_string(cpy_ptr);
    }

    emit_block(outfile, types_post);
}

void emit_ast_header(void) {

    String* str = create_string(get_cmdline("ast_name"));
    append_string_str(str, ".h");

    outfile = fopen(raw_string(str), "w");
    if(outfile == NULL) {
        fprintf(stderr, "Fatal error: cannot open output file: '%s': %s\n",
                raw_string(str), strerror(errno));
        exit(1);
    }
    clear_string(str);

    time_t t = time(NULL);

    fprintf(outfile, "/*\n");
    char* tmp = ctime(&t);
    tmp[strlen(tmp)-1] = '\0';
    fprintf(outfile, " * File generated on %s.\n", tmp);
    emit_block(outfile, file_pre);

    emit_type_list();

    emit_ds();
    emit_protos();

    emit_block(outfile, file_post);
    destroy_string(str);
}

