#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

static int current_line = 1;

token_t get_next_token(char **input) {
    while (isspace(**input)) {
        if (**input == '\n') current_line++; // O Lexer cuida disso
        (*input)++;
    }
    
    token_t token;
    token.line = current_line;
    if (**input == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    // Se for número
    if (isdigit(**input)) {
        char *start = *input;
        while (isdigit(**input)) (*input)++;
        
        int len = *input - start;
        strncpy(token.text, start, len);
        token.text[len] = '\0';
        token.value = atoi(token.text);
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Se for palavra (comando)
    if (isalpha(**input)) {
        char *start = *input;
        while (isalnum(**input)) (*input)++;
        
        int len = *input - start;
        strncpy(token.text, start, len);
        token.text[len] = '\0';
        token.type = TOKEN_COMMAND;
        return token;
    }

    if (**input == '(' || **input == ')' || **input == ',' || **input == '=') {
        token.type = TOKEN_SYMBOL;
        token.text[0] = **input;
        token.text[1] = '\0';
        (*input)++;
        return token;
    }

    token.type = TOKEN_ERROR;
    (*input)++;
    return token;
}