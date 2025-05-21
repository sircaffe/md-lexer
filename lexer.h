#ifndef LEXER_H
#define LEXER_H

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
    Token *tokens;
    size_t count;
    size_t capacity;
} Tokens;

typedef struct {
    char *data;
    size_t size;
} Buffer;

void read_entire_file(Buffer *buffer, char *file_path);
void append_token(Tokens *tokens, char *inside, enum Token_Type type);

#ifdef LEXER_IMPLEMENTATION

#define usage(argc, argv) do { printf("Usage: %s <path-to-file>\n", argv[0]); } while(0);

void read_entire_file(Buffer *buffer, char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Unable to open file `%s`\n", file_path);
        exit(1);
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
    }

#ifdef DEBUG_INFO
    printf("DEBUG: Stored bytes into buffer\n");
#endif

    fclose(f);
}

void append_token(Tokens *tokens, char *inside, enum Token_Type type) {
    // initialise token dynamic array
    if (tokens->capacity == 0) {
        tokens->tokens = malloc(sizeof(Token));
        if (tokens->tokens == NULL) {
            fprintf(stderr, "ERROR: Unable to initialise token dynamic array\n");
            exit(1);
        }

        tokens->capacity = 1;
    }

    if (tokens->count >= tokens->capacity) {
        tokens->tokens = realloc(tokens->tokens, (sizeof(Token) * tokens->count) * 2);
        if (tokens->tokens == NULL) {
            fprintf(stderr, "ERROR: Unable to resize token dynamic array\n");
            exit(1);
        }

        tokens->capacity *= 2;
    }
    
    tokens->tokens[tokens->count].inside = malloc(strlen(inside));
    strcpy(tokens->tokens[tokens->count].inside, inside);
    tokens->tokens[tokens->count].type = type;
    tokens->count += 1;
   
    if (tokens->count > tokens->capacity) {
        fprintf(stderr, "ERROR: Count bigger than capacity. Shouldn't happen\n");
        exit(1);
    }
}

// TODO: Implement function that expects current and subsequent tokens
void lex_from_buffer(Tokens *tokens, Buffer *buffer) {
    size_t token_count = 0;
    size_t line_ruler = 0;

    for (size_t cursor = 0; cursor < buffer->size; ++cursor) {
        if (buffer->data[cursor] == '\n') {
            for (size_t j = 0; j < line_ruler; ++j) {
                size_t current = (cursor - line_ruler) + j;
                if (buffer->data[current] == '#' && buffer->data[current + 1] == ' ') {
                    // TODO: Fix bug that cuts last character of first token content
                    char c[line_ruler - 2];
                    strncpy(c, &buffer->data[current + 2], line_ruler - 2);
                    c[line_ruler - 3] = '\0';
                    
                    append_token(tokens, c, HASH);

                    token_count += 1;
                }
                if (buffer->data[current] == '-' && buffer->data[current + 1] == ' ') {
                    char c[line_ruler];
                    size_t sz = line_ruler - 3;
                    strncpy(c, &buffer->data[current + 2], sz);
                    c[sz] = '\0';

                    append_token(tokens, c, LIST);
                    token_count += 1;
                }
            }
            line_ruler = 0;
        }
        line_ruler += 1;
    }
}

#endif // NOB_IMPLEMENTATION
#endif // LEXER_H
