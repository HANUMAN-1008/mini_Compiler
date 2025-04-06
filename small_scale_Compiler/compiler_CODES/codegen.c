#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

static void emit_expr(Node *node, FILE *out);
static void emit_assign(const char *id, Node *expr, FILE *out);
static void emit_print(Node *expr, FILE *out);

void generate_assembly(Node *ast, FILE *out) {
    // Initialize sections
    fprintf(out, "section .text\n");
    fprintf(out, "global main\n");
    fprintf(out, "extern printf, exp\n");
    
    // Data section
    fprintf(out, "section .data\n");
    fprintf(out, "fmt db \"%%f\", 10, 0\n");
    fprintf(out, "one dq 1.0\n");
    fprintf(out, "neg_one dq -1.0\n");
    
    // BSS section for variables
    fprintf(out, "section .bss\n");
    
    // Text section with proper entry point
    fprintf(out, "section .text\n");
    fprintf(out, "main:\n");
    fprintf(out, "    push rbp\n");
    fprintf(out, "    mov rbp, rsp\n");
    fprintf(out, "    sub rsp, 32\n");  // Shadow space + alignment

    // Generate code
    for (Node *stmt = ast; stmt; stmt = stmt->next) {
        switch (stmt->type) {
            case NODE_ASSIGNMENT:
                emit_assign(stmt->data.assignment.id, stmt->data.assignment.expr, out);
                break;
            case NODE_PRINT:
                emit_print(stmt->data.print_expr, out);
                break;
            default:
                fprintf(stderr, "Error: Unexpected node type in statement position\n");
                exit(1);
        }
    }

    // Proper exit
    fprintf(out, "    mov rsp, rbp\n");
    fprintf(out, "    pop rbp\n");
    fprintf(out, "    xor eax, eax\n");  // Return 0
    fprintf(out, "    ret\n");
}

static void emit_expr(Node *node, FILE *out) {
    switch (node->type) {
        case NODE_NUMBER:
            fprintf(out, "    mov rax, __float64__(%f)\n", node->data.number);
            fprintf(out, "    movq xmm0, rax\n");
            break;
            
        case NODE_IDENTIFIER:
            fprintf(out, "    movsd xmm0, [%s]\n", node->data.identifier);
            break;
            
        case NODE_BIN_OP:
            emit_expr(node->data.bin_op.left, out);
            fprintf(out, "    movsd [rsp+16], xmm0\n");
            emit_expr(node->data.bin_op.right, out);
            
            switch (node->data.bin_op.op) {
                case TOKEN_PLUS:
                    fprintf(out, "    addsd xmm0, [rsp+16]\n");
                    break;
                case TOKEN_MINUS:
                    fprintf(out, "    movsd xmm1, [rsp+16]\n");
                    fprintf(out, "    subsd xmm1, xmm0\n");
                    fprintf(out, "    movsd xmm0, xmm1\n");
                    break;
                case TOKEN_MULTIPLY:
                    fprintf(out, "    mulsd xmm0, [rsp+16]\n");
                    break;
                case TOKEN_DIVIDE:
                    fprintf(out, "    movsd xmm1, [rsp+16]\n");
                    fprintf(out, "    divsd xmm1, xmm0\n");
                    fprintf(out, "    movsd xmm0, xmm1\n");
                    break;
                default:
                    fprintf(stderr, "Error: Invalid binary operator\n");
                    exit(1);
            }
            break;
            
        case NODE_SIGMOID:
            emit_expr(node->data.sigmoid_arg, out);
            fprintf(out, "    movsd xmm1, xmm0\n");
            fprintf(out, "    movsd xmm0, [neg_one]\n");
            fprintf(out, "    mulsd xmm0, xmm1\n");
            fprintf(out, "    call exp\n");
            fprintf(out, "    addsd xmm0, [one]\n");
            fprintf(out, "    movsd xmm1, [one]\n");
            fprintf(out, "    divsd xmm1, xmm0\n");
            fprintf(out, "    movsd xmm0, xmm1\n");
            break;
            
        case NODE_ASSIGNMENT:
        case NODE_PRINT:
            fprintf(stderr, "Error: Unexpected statement node in expression\n");
            exit(1);
    }
}

static void emit_assign(const char *id, Node *expr, FILE *out) {
    // Declare variable in BSS
    fprintf(out, "section .bss\n");
    fprintf(out, "%s: resq 1\n", id);
    fprintf(out, "section .text\n");
    
    emit_expr(expr, out);
    fprintf(out, "    movsd [%s], xmm0\n", id);
}

static void emit_print(Node *expr, FILE *out) {
    emit_expr(expr, out);
    fprintf(out, "    mov rdi, fmt\n");
    fprintf(out, "    mov rax, 1\n");  // 1 FP arg in XMM0
    fprintf(out, "    call printf\n");
}