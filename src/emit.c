/**
 * @file emit.c
 *
 * @brief This is the over all emitter that retrieves the emitter
 * configuration from the command line parser and calls the other emitters.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 2024-08-07
 * @copyright Copyright (c) 2024
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "emit_ast_header.h"
#include "emit_ast_source.h"
#include "emit_parse_header.h"
#include "emit_parse_source.h"

void emit(void) {

    emit_ast_header();
    emit_ast_source();
    emit_parse_header();
    emit_parse_source();
}

void emit_block(FILE* fh, const char* const* block) {

    for(int i = 0; block[i] != NULL; i++) {
        fprintf(fh, "%s\n", block[i]);
    } 
}