#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

void generate_assembly(Node *ast, FILE *out);

#endif