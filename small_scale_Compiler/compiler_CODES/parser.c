#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

static Token *current_token;

Node *parse_statement(Token **tokens) {
    current_token = *tokens;
    Node *node = NULL;
    
    if (current_token->type == TOKEN_PRINT) {
        current_token = current_token->next;
        node = malloc(sizeof(Node));
        node->type = NODE_PRINT;
        node->data.print_expr = parse_expression(&current_token);
    } 
    else if (current_token->type == TOKEN_IDENTIFIER && 
             current_token->next && 
             current_token->next->type == TOKEN_EQUALS) {
        char *id = strdup(current_token->identifier);
        current_token = current_token->next->next;
        node = malloc(sizeof(Node));
        node->type = NODE_ASSIGNMENT;
        node->data.assignment.id = id;
        node->data.assignment.expr = parse_expression(&current_token);
    }
    else {
        fprintf(stderr, "Invalid statement\n");
        exit(1);
    }
    
    *tokens = current_token;
    return node;
}

// Rest of your existing parser.c implementation...

Node *parse(Token **tokens) {
    current_token = *tokens;
    Node *ast = NULL, **current = &ast;
    
    while (current_token->type != TOKEN_EOF) {
        *current = parse_statement(&current_token);
        current = &(*current)->next;
    }
    
    *tokens = current_token;
    return ast;
}

Node *parse_expression(Token **tokens) {
    current_token = *tokens;
    Node *node = parse_term(&current_token);
    while (current_token->type == TOKEN_PLUS || current_token->type == TOKEN_MINUS) {
        TokenType op = current_token->type;
        current_token = current_token->next;
        Node *right = parse_term(&current_token);
        Node *new_node = malloc(sizeof(Node));
        new_node->type = NODE_BIN_OP;
        new_node->data.bin_op.op = op;
        new_node->data.bin_op.left = node;
        new_node->data.bin_op.right = right;
        node = new_node;
    }
    *tokens = current_token;
    return node;
}

Node *parse_term(Token **tokens) {
    current_token = *tokens;
    Node *node = parse_factor(&current_token);
    while (current_token->type == TOKEN_MULTIPLY || current_token->type == TOKEN_DIVIDE) {
        TokenType op = current_token->type;
        current_token = current_token->next;
        Node *right = parse_factor(&current_token);
        Node *new_node = malloc(sizeof(Node));
        new_node->type = NODE_BIN_OP;
        new_node->data.bin_op.op = op;
        new_node->data.bin_op.left = node;
        new_node->data.bin_op.right = right;
        node = new_node;
    }
    *tokens = current_token;
    return node;
}

Node *parse_factor(Token **tokens) {
    current_token = *tokens;
    Node *node = NULL;
    
    if (current_token->type == TOKEN_NUMBER) {
        node = malloc(sizeof(Node));
        node->type = NODE_NUMBER;
        node->data.number = current_token->number_value;
        current_token = current_token->next;
    }
    else if (current_token->type == TOKEN_IDENTIFIER) {
        node = malloc(sizeof(Node));
        node->type = NODE_IDENTIFIER;
        node->data.identifier = strdup(current_token->identifier);
        current_token = current_token->next;
    }
    else if (current_token->type == TOKEN_LPAREN) {
        current_token = current_token->next;
        node = parse_expression(&current_token);
        if (current_token->type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')'\n");
            exit(1);
        }
        current_token = current_token->next;
    }
    else if (current_token->type == TOKEN_SIGMOID) {
        current_token = current_token->next;
        if (current_token->type != TOKEN_LPAREN) {
            fprintf(stderr, "Expected '(' after 'sigmoid'\n");
            exit(1);
        }
        current_token = current_token->next;
        Node *arg = parse_expression(&current_token);
        if (current_token->type != TOKEN_RPAREN) {
            fprintf(stderr, "Expected ')' after sigmoid argument\n");
            exit(1);
        }
        current_token = current_token->next;
        node = malloc(sizeof(Node));
        node->type = NODE_SIGMOID;
        node->data.sigmoid_arg = arg;
    }
    else {
        fprintf(stderr, "Unexpected token in factor\n");
        exit(1);
    }
    
    *tokens = current_token;
    return node;
}

// Keep your existing free_ast implementation

void free_ast(Node *ast) {
    if (!ast) return;
    switch (ast->type) {
        case NODE_ASSIGNMENT:
            free(ast->data.assignment.id);
            free_ast(ast->data.assignment.expr);
            break;
        case NODE_PRINT:
            free_ast(ast->data.print_expr);
            break;
        case NODE_BIN_OP:
            free_ast(ast->data.bin_op.left);
            free_ast(ast->data.bin_op.right);
            break;
        case NODE_IDENTIFIER:
            free(ast->data.identifier);
            break;
        case NODE_SIGMOID:
            free_ast(ast->data.sigmoid_arg);
            break;
        default:
            break;
    }
    free_ast(ast->next);
    free(ast);
}