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
#include <stdio.h>
#include "ast.h"

int production_flag = 0;
int in_group_flag = 0;

/*
 * This is a function that does nothing. One could delete this and simply 
 * don't include it in the pre or post function.
 */
static void dummy(AstNode* node) {

    (void)node;
}

static void pre_error(AstNode* node) {

    (void)node;
    fprintf(stderr, "FATAL: A really impossible and bad thing happened: regurg: pre.\n");
    abort();
}

static void post_error(AstNode* node) {
    
    (void)node;
    fprintf(stderr, "FATAL: A really impossible and bad thing happened: regurg: post.\n");
    abort();
}

static void pre_terminal(AstNode* ptr) {

    printf("%s ", raw_string(((ast_terminal_t*)ptr)->tok));
}

static void pre_reference(AstNode* ptr) {

    printf("%s ", raw_string(((ast_terminal_t*)ptr)->tok));
}

static void pre_zero_or_one(AstNode* ptr) {

    (void)ptr;
    printf("( ");
}

static void post_zero_or_one(AstNode* ptr) {

    (void)ptr;
    printf(")? ");
}

static void pre_zero_or_more(AstNode* ptr) {

    (void)ptr;
    printf("( ");
}

static void post_zero_or_more(AstNode* ptr) {

    (void)ptr;
    printf(")* ");
}

static void pre_one_or_more(AstNode* ptr) {

    (void)ptr;
    printf("( ");
}

static void post_one_or_more(AstNode* ptr) {

    (void)ptr;
    printf(")+ ");
}

static void pre_rule(AstNode* ptr) {

    printf("%s\n", raw_string(((ast_rule_t*)ptr)->name));
}

static void post_rule(AstNode* ptr) {
    
    (void)ptr;
    printf("\n    ;\n\n");
    production_flag = 0;
}

static void pre_production(AstNode* ptr) {

    (void)ptr;
    if(production_flag == 0) {
        printf("    : ");
        production_flag = 1;
    }
    else if(in_group_flag == 0) {
        printf("\n    | ");
    }
}

static void post_production(AstNode* ptr) {

    (void)ptr;
    //production_flag = 0;
}

static void pre_group(AstNode* ptr) {

    (void)ptr;
    in_group_flag = 1;
}

static void post_group(AstNode* ptr) {

    (void)ptr;
    in_group_flag = 0;
}

static void regurg_pre(AstNode* node) {

    void (*func)(AstNode*) = (node->type == AST_TERMINAL)? pre_terminal:
        (node->type == AST_NTERM_REFERENCE)? pre_reference:
        (node->type == AST_ZERO_OR_ONE)? pre_zero_or_one:
        (node->type == AST_ONE_OR_MORE)? pre_one_or_more:
        (node->type == AST_ZERO_OR_MORE)? pre_zero_or_more:
        (node->type == AST_GROUP)? pre_group:
        (node->type == AST_GRAMMAR)? dummy:
        (node->type == AST_RULE)? pre_rule:
        (node->type == AST_PRODUCTION_LIST)? dummy:
        (node->type == AST_PRODUCTION)? pre_production:
        (node->type == AST_PROD_ELEM)? dummy: pre_error;

    (*func)(node);
}

static void regurg_post(AstNode* node) {

    void (*func)(AstNode*) = (node->type == AST_TERMINAL)? dummy:
        (node->type == AST_NTERM_REFERENCE)? dummy:
        (node->type == AST_ZERO_OR_ONE)? post_zero_or_one:
        (node->type == AST_ONE_OR_MORE)? post_one_or_more:
        (node->type == AST_ZERO_OR_MORE)? post_zero_or_more:
        (node->type == AST_GROUP)? post_group:
        (node->type == AST_GRAMMAR)? dummy:
        (node->type == AST_RULE)? post_rule:
        (node->type == AST_PRODUCTION_LIST)? dummy:
        (node->type == AST_PRODUCTION)? post_production:
        (node->type == AST_PROD_ELEM)? dummy: post_error;

    (*func)(node);
}

/**
 * @brief Public interface.
 * 
 */
void regurg(void) {

    traverse_ast(regurg_pre, regurg_post);
}

