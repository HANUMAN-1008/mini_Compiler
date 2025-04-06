%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* Structure for symbol table entries */
typedef struct symbol {
    char *name;
    double value;
    struct symbol *next;
} symbol;

symbol *symtab = NULL;

/* Function declarations */
double lookup(char *s);
void install(char *s, double val);
int yylex(void);
void yyerror(const char *s);
double sigmoid(double x);
int yywrap() {
    return 1; // Indicate that there is no more input
}
%}

/* Define the union to pass values from lex to bison */
%union {
    double val;
    char *str;
}

/* Define tokens. NUMBER returns a double, IDENT returns a string */
%token <val> NUMBER
%token <str> IDENT
%token EXIT SIGMOID
%type <val> expression
%type <val> statement
%type <val> input

/* Define operator precedence and associativity */
%left '+' '-'
%left '*' '/'
%right UMINUS '^'

%%
/* Grammar rules and actions */
input:
    /* empty */{ $$ = 0; }
    | input line
;

line:
      '\n'
    | statement '\n'
;

statement:
      expression {
          printf("Result: %lf\n", $1);
          $$ = $1;
      }
    | IDENT '=' expression {
          install($1, $3);
          $$ = $3;
          free($1);
      }
    | EXIT {
          printf("Exiting program...\n");
          exit(0); // Exit the program immediately.
      }
    ;


expression:
      expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { 
                                    if ($3 == 0) { 
                                      yyerror("division by zero"); 
                                      $$ = 0; 
                                    } else {
                                      $$ = $1 / $3; 
                                    }
                                  }
    | expression '^' expression {
                                    if ($3==0 && $1==0) {
                                        yyerror("exponentiation of zero with zero");
                                        $$ = 0;
                                    } else {
                                      $$ = pow($1, $3);
                                    }
                                  }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')'        { $$ = $2; }
    | NUMBER                    { $$ = $1; }
    | IDENT                     { $$ = lookup($1); free($1); }
    | SIGMOID '(' expression ')' { $$ = sigmoid($3); }  /* Sigmoid function call */
;
%%

/* Sigmoid function implementation */
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

/* Lookup a variable in the symbol table.
   If the variable is not found, print an error message. */
double lookup(char *s) {
    symbol *sym = symtab;
    while (sym != NULL) {
        if (strcmp(sym->name, s) == 0) {
            return sym->value;
        }
        sym = sym->next;
    }
    fprintf(stderr, "Undefined variable: %s\n", s);
    return 0;
}

/* Install a variable in the symbol table.
   If the variable already exists, update its value. */
void install(char *s, double val) {
    symbol *sym = symtab;
    while (sym != NULL) {
        if (strcmp(sym->name, s) == 0) {
            sym->value = val;
            return;
        }
        sym = sym->next;
    }
    /* Create a new symbol table entry */
    symbol *newSym = (symbol *)malloc(sizeof(symbol));
    newSym->name = strdup(s);
    newSym->value = val;
    newSym->next = symtab;
    symtab = newSym;
}

/* Error handling function for Bison */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

/* Main function - starts the parser */
int main(void) {
    printf("1. Define all the variables you gonna use\n");
    printf("2. Define the arithmetic equation you gonna use\n");
    printf("3. Type the variable to see the answer\n");
    printf("4. Use sigmoid(x) to calculate the sigmoid function\n");
    return yyparse();
}