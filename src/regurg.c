/**
 * @file regurg.c
 *
 * @brief Running this proves that the input grammar was read correctly and
 * properly stored in the AST.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-08-03
 * @copyright Copyright (c) 2024
 *
 */
#include "ast.h"
#include <stdio.h>

static int production_flag = 0;
static int in_group_flag   = 0;
static FILE* outfile       = NULL;

/*
 * This is a function that does nothing. One could delete this and simply
 * don't include it in the pre or post function.
 */
static int dummy(void) {

    return 0;
}

static int error(const char* fn, int st) {

    fprintf(stderr, "Fatal error: invalid state in %s: %d.\n", fn, st);
    abort();
}

static int pre_terminal(AstNode* ptr) {

    fprintf(outfile, "%s ", raw_string(((ast_terminal_t*)ptr)->tok));
    return 0;
}

static int pre_reference(AstNode* ptr) {

    fprintf(outfile, "%s ", raw_string(((ast_terminal_t*)ptr)->tok));
    return 0;
}

static int pre_zero_or_one(void) {

    fprintf(outfile, "( ");
    return 0;
}

static int post_zero_or_one(void) {

    fprintf(outfile, ")? ");
    return 0;
}

static int pre_zero_or_more(void) {

    fprintf(outfile, "( ");
    return 0;
}

static int post_zero_or_more() {

    fprintf(outfile, ")* ");
    return 0;
}

static int pre_one_or_more() {

    fprintf(outfile, "( ");
    return 0;
}

static int post_one_or_more() {

    fprintf(outfile, ")+ ");
    return 0;
}

static int pre_rule(AstNode* ptr) {

    fprintf(outfile, "%s\n", raw_string(((ast_rule_t*)ptr)->name));
    return 0;
}

static int post_rule() {

    fprintf(outfile, "\n    ;\n\n");
    production_flag = 0;
    return 0;
}

static int pre_production() {

    if(production_flag == 0) {
        fprintf(outfile, "    : ");
        production_flag = 1;
    }
    else if(in_group_flag == 0) {
        fprintf(outfile, "\n    | ");
    }
    return 0;
}

static int post_production() {

    // production_flag = 0;
    return 0;
}

static int pre_group() {

    in_group_flag = 1;
    return 0;
}

static int post_group() {

    in_group_flag = 0;
    return 0;
}

static int regurg_pre(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? pre_terminal(node) :
            (node->type == AST_NTERM_REFERENCE) ? pre_reference(node) :
            (node->type == AST_ZERO_OR_ONE)     ? pre_zero_or_one() :
            (node->type == AST_ONE_OR_MORE)     ? pre_one_or_more() :
            (node->type == AST_ZERO_OR_MORE)    ? pre_zero_or_more() :
            (node->type == AST_GROUP)           ? pre_group() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? pre_rule(node) :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? pre_production() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  error(__func__, node->type);
}

static int regurg_post(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? dummy() :
            (node->type == AST_NTERM_REFERENCE) ? dummy() :
            (node->type == AST_ZERO_OR_ONE)     ? post_zero_or_one() :
            (node->type == AST_ONE_OR_MORE)     ? post_one_or_more() :
            (node->type == AST_ZERO_OR_MORE)    ? post_zero_or_more() :
            (node->type == AST_GROUP)           ? post_group() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? post_rule() :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? post_production() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  error(__func__, node->type);
}

/**
 * @brief Public interface.
 *
 */
void regurg(void) {

    outfile = stdout;

    traverse_ast(regurg_pre, regurg_post);
}
