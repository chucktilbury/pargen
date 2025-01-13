
// #include <errno.h>
#include <stdio.h>
// #include <string.h>

#include "scanner.h"

int main(int argc, char** argv) {

    if(argc < 2) {
        printf("syntax: %s filename\n", argv[0]);
        return 1;
    }

    init_scanner(argv[1]);
    token_t* tok = consume_token();
    for(; tok->type != END_OF_INPUT; tok = consume_token()) {
        printf("%s: %s: %s\n", tok_type_to_str(tok), tok->text, tok->name);
    }
    return 0;


    uninit_scanner();
    return 0;
}
