#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

typedef enum {
    NODE_ASSIGNMENT, NODE_PRINT, NODE_BIN_OP,
    NODE_NUMBER, NODE_IDENTIFIER, NODE_SIGMOID
} NodeType;

typedef struct Node {
    NodeType type;
    union {
        struct { char *id; struct Node *expr; } assignment;
        struct Node *print_expr;
        struct { TokenType op; struct Node *left, *right; } bin_op;
        double number;
        char *identifier;
        struct Node *sigmoid_arg;
    } data;
    struct Node *next;
} Node;
Node *parse(Token **tokens);
Node *parse_statement(Token **tokens);  // Add this line
Node *parse_expression(Token **tokens);
Node *parse_term(Token **tokens);
Node *parse_factor(Token **tokens);
void free_ast(Node *ast);

#endif