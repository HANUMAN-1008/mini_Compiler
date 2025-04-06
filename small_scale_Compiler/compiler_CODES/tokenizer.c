#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tokenizer.h"

Token *new_token(TokenType type) {
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->next = NULL;
    return token;
}
Token *tokenize(const char *input) {
    Token *head = NULL, **current = &head;
    const char *p = input;

    while (*p) {
        if (isspace(*p)) { p++; continue; }

        if (isdigit(*p) || *p == '.') {
            char *end;
            double num = strtod(p, &end);
            *current = malloc(sizeof(Token));
            (*current)->type = TOKEN_NUMBER;
            (*current)->number_value = num;
            (*current)->next = NULL;
            current = &(*current)->next;
            p = end;
            continue;
        }

        if (isalpha(*p) || *p == '_') {
            const char *start = p;
            while (isalnum(*p) || *p == '_') p++;
            size_t len = p - start;
            char *name = strndup(start, len);

            *current = malloc(sizeof(Token));
            if (strcmp(name, "print") == 0) {
                (*current)->type = TOKEN_PRINT;
            } else if (strcmp(name, "sigmoid") == 0) {
                (*current)->type = TOKEN_SIGMOID;
            } else {
                (*current)->type = TOKEN_IDENTIFIER;
                (*current)->identifier = name;
            }
            (*current)->next = NULL;
            current = &(*current)->next;
            continue;
        }

        switch (*p) {
            case '+': *current = new_token(TOKEN_PLUS); break;
            case '-': *current = new_token(TOKEN_MINUS); break;
            case '*': *current = new_token(TOKEN_MULTIPLY); break;
            case '/': *current = new_token(TOKEN_DIVIDE); break;
            case '(': *current = new_token(TOKEN_LPAREN); break;
            case ')': *current = new_token(TOKEN_RPAREN); break;
            case '=': *current = new_token(TOKEN_EQUALS); break;
            default:
                fprintf(stderr, "Unknown character: %c\n", *p);
                exit(1);
        }
        current = &(*current)->next;
        p++;
    }

    *current = malloc(sizeof(Token));
    (*current)->type = TOKEN_EOF;
    (*current)->next = NULL;
    return head;
}

void free_tokens(Token *tokens) {
    while (tokens) {
        Token *next = tokens->next;
        if (tokens->type == TOKEN_IDENTIFIER) free(tokens->identifier);
        free(tokens);
        tokens = next;
    }
}