#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {
    TOKEN_EOF, TOKEN_NUMBER, TOKEN_IDENTIFIER,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE,
    TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_EQUALS, TOKEN_PRINT, TOKEN_SIGMOID
} TokenType;

typedef struct Token {
    TokenType type;
    double number_value;
    char *identifier;
    struct Token *next;
} Token;
Token *new_token(TokenType type);

Token *tokenize(const char *input);
void free_tokens(Token *tokens);

#endif