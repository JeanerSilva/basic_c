#pragma once

typedef enum {
    TOKEN_NUMBER,
    TOKEN_COMMAND,
    TOKEN_EOF,
    TOKEN_ERROR
} token_type_t;

typedef struct {
    token_type_t type;
    char text[32];
    int value; // Para armazenar números convertidos
} token_t;

// Protótipo para ler o próximo token de uma string
token_t get_next_token(char **input);