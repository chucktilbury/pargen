/**
 * @file emit_parse_source.c
 *
 * @brief This emits the parser source code. It is a pass on the AST that
 * emits the source file and the header file.
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

static int parse_source_pre(AstNode* node) {

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

static int parse_source_post(AstNode* node) {

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

void emit_parse_source(void) {

    traverse_ast(parse_source_pre, parse_source_post);
}
