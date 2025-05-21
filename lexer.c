#include <stdio.h>

#define LEXER_IMPLEMENTATION
#include "lexer.h"

int main(int argc, char **argv) {
    char *file_path = argv[1];
    if (argc > 2) {
        usage(argc, argv);
        fprintf(stderr, "ERROR: Too many arguments\n");
        return 1; 
    } else if (argc < 2) {
        usage(argc, argv);
        fprintf(stderr, "ERROR: Too few arguments\n");
        return 1;
    }

    Buffer buffer = {0};
    read_entire_file(&buffer, file_path);

    Tokens tokens = {0};
    lex_from_buffer(&tokens, &buffer);

    for (size_t i = 0; i < tokens.count; ++i) {
        if (tokens.tokens[i].type == HASH) {
            printf("> Token %zu: HASH(%s)\n", i, tokens.tokens[i].inside);
        } else if (tokens.tokens[i].type == LIST) {
            printf("> Token %zu: LIST(%s)\n", i, tokens.tokens[i].inside);
        }
    }

#ifdef DEBUG_INFO
    printf("DEBUG: %zu tokens found\n", tokens.count);
#endif

    free(buffer.data);
    free(tokens.tokens);

    return 0;
}
