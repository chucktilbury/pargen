/**
 * @file emit_ast.c
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

#include "ast.h"

static int error(const char* fn, int state) {
    fprintf(stderr, "Fatal internal error: invalid state in %s: %d\n", fn, state);
    abort();
}

static int dummy(void) {
    // do nothing.
    return 0;
}

static int ast_source_pre(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? dummy() :
            (node->type == AST_NTERM_REFERENCE) ? dummy() :
            (node->type == AST_ZERO_OR_ONE)     ? dummy() :
            (node->type == AST_ONE_OR_MORE)     ? dummy() :
            (node->type == AST_ZERO_OR_MORE)    ? dummy() :
            (node->type == AST_GROUP)           ? dummy() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? dummy() :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? dummy() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  error(__func__, node->type);
}

static int ast_source_post(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? dummy() :
            (node->type == AST_NTERM_REFERENCE) ? dummy() :
            (node->type == AST_ZERO_OR_ONE)     ? dummy() :
            (node->type == AST_ONE_OR_MORE)     ? dummy() :
            (node->type == AST_ZERO_OR_MORE)    ? dummy() :
            (node->type == AST_GROUP)           ? dummy() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? dummy() :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? dummy() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  error(__func__, node->type);
}

static int ast_header_pre(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? dummy() :
            (node->type == AST_NTERM_REFERENCE) ? dummy() :
            (node->type == AST_ZERO_OR_ONE)     ? dummy() :
            (node->type == AST_ONE_OR_MORE)     ? dummy() :
            (node->type == AST_ZERO_OR_MORE)    ? dummy() :
            (node->type == AST_GROUP)           ? dummy() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? dummy() :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? dummy() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  error(__func__, node->type);
}

static int ast_header_post(AstNode* node) {

    return (node->type == AST_TERMINAL)         ? dummy() :
            (node->type == AST_NTERM_REFERENCE) ? dummy() :
            (node->type == AST_ZERO_OR_ONE)     ? dummy() :
            (node->type == AST_ONE_OR_MORE)     ? dummy() :
            (node->type == AST_ZERO_OR_MORE)    ? dummy() :
            (node->type == AST_GROUP)           ? dummy() :
            (node->type == AST_GRAMMAR)         ? dummy() :
            (node->type == AST_RULE)            ? dummy() :
            (node->type == AST_PRODUCTION_LIST) ? dummy() :
            (node->type == AST_PRODUCTION)      ? dummy() :
            (node->type == AST_PROD_ELEM)       ? dummy() :
                                                  error(__func__, node->type);
}

void emit_ast_header(void) {

    traverse_ast(ast_header_pre, ast_header_post);
}

void emit_ast_source(void) {

    traverse_ast(ast_source_pre, ast_source_post);
}
