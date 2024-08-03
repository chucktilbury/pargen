/**
 * @file regurg.c
 * 
 * @brief 
 * 
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-08-03
 * @copyright Copyright (c) 2024
 * 
 */

#include "ast.h"

void dummy(AstNode* node) {

    (void)node;
}

void regurg_pre(AstNode* node) {

    void (*func)(AstNode*) = (node->type == AST_TERMINAL)? terminal:
        (node->type == AST_NTERM_REFERENCE)? dummy:
        (node->type == AST_ZERO_OR_ONE)? dummy:
        (node->type == AST_ONE_OR_MORE)? dummy:
        (node->type == AST_ZERO_OR_MORE)? dummy:
        (node->type == AST_GROUP)? dummy:
        (node->type == AST_GRAMMAR)? dummy:
        (node->type == AST_RULE)? dummy:
        (node->type == AST_PRODUCTION_LIST)? dummy:
        (node->type == AST_PRODUCTION)? dummy:
        (node->type == AST_PROD_ELEM)? dummy: dummy;

    (*func)(node);
}

void regurg_post(AstNode* node) {

    void (*func)(AstNode*) = (node->type == AST_TERMINAL)? terminal:
        (node->type == AST_NTERM_REFERENCE)? dummy:
        (node->type == AST_ZERO_OR_ONE)? dummy:
        (node->type == AST_ONE_OR_MORE)? dummy:
        (node->type == AST_ZERO_OR_MORE)? dummy:
        (node->type == AST_GROUP)? dummy:
        (node->type == AST_GRAMMAR)? dummy:
        (node->type == AST_RULE)? dummy:
        (node->type == AST_PRODUCTION_LIST)? dummy:
        (node->type == AST_PRODUCTION)? dummy:
        (node->type == AST_PROD_ELEM)? dummy: dummy;

    (*func)(node);
}

/**
 * @brief Public interface.
 * 
 */
void regurg(void) {

    traverse_ast(regurg_pre, regurg_post);
}