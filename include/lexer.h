#pragma once

typedef enum {
    TOKEN_NUMBER,
    TOKEN_COMMAND,
    TOKEN_SYMBOL,
    TOKEN_EOF,
    TOKEN_ERROR
} token_type_t;

typedef struct {
    token_type_t type;
    char text[32];
    int value;
    int line; // Necessário para o tratamento de erros
} token_t;

token_t get_next_token(char **input);