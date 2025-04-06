#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    fseek(input, 0, SEEK_END);
    long size = ftell(input);
    rewind(input);
    char *source = malloc(size + 1);
    fread(source, 1, size, input);
    source[size] = '\0';
    fclose(input);

    Token *tokens = tokenize(source);
    Node *ast = parse(&tokens);
    FILE *output = fopen("output.asm", "w");
    generate_assembly(ast, output);
    fclose(output);

    system("nasm -f elf64 output.asm -o output.o");
    system("gcc -no-pie output.o -lm -o output");

    free_tokens(tokens);
    free_ast(ast);
    free(source);
    return 0;
}