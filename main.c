#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define DEBUG_INFO

enum Token_Type {
    STRING = 0,
    HASH,
    LIST,
};

typedef struct {
    char *inside;
    enum Token_Type type;
} Token;

typedef struct {
    char *data;
    size_t size;
} Buffer;

bool read_entire_file(Buffer *buffer, char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Unable to open file `%s`\n", file_path);
        return false;
    }

    fseek(f, 0, SEEK_END);
    buffer->size = ftell(f);
    rewind(f);

#ifdef DEBUG_INFO
    printf("DEBUG: Loaded `%s` with %zu bytes\n", file_path, buffer->size);
#endif
    
    buffer->data = malloc(sizeof(char) * buffer->size);
    size_t bytes_count = fread(buffer->data, sizeof(char), buffer->size, f);

    if (bytes_count < buffer->size) {
        fprintf(stderr, "ERROR: Bytes read smaller than file size: %zu and %zu\n", bytes_count, buffer->size);
        return false;
    }

#ifdef DEBUG_INFO
    printf("DEBUG: Stored bytes into buffer\n");
#endif

    fclose(f);
    return true;
}

Token *lexer_init(Buffer *buffer) {
    size_t cursor = 0;
    size_t token_count = 0;

    Token *tokens = malloc(sizeof(Token) * 16);
    
    for (size_t i = 0; i < buffer->size; ++i) {
        if (buffer->data[i] == '\n') {
            for (size_t j = 0; j < cursor; ++j) {
                if (buffer->data[(i - cursor) + j] == '#' && buffer->data[(i - cursor) + j + 1] == ' ') {
                    printf("DEBUG: FOUND HEADER 1\n");
                    token_count += 1;
                }
                if (buffer->data[(i - cursor) + j] == '-' && buffer->data[(i - cursor) + j + 1] == ' ') {
                    printf("DEBUG: FOUND LIST\n");
                    token_count += 1;
                }           }
            cursor = 0;
        }
        cursor += 1;
    }
    
    return tokens;
}

int main(void) {
    char *file_path = "test.md";

    Buffer buffer = {0};
    if (!read_entire_file(&buffer, file_path)) return 1;
    
    Token *tokens = lexer_init(&buffer);
    (void) tokens;

#ifdef DEBUG_INFO
    printf("\n------ Buffer Content ------\n%s\n----------------------------\n", buffer.data);
#endif

    free(buffer.data);

    return 0;
}
