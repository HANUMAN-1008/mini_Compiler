%{
#include "calc.tab.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

double sigmoid(double x);
%}

/* Regular expression definitions */
DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*

%%
"exit"    { return EXIT; }
"sigmoid" { return SIGMOID; }  /* Add sigmoid keyword */
{DIGIT}+(\.[0-9]+)?   { 
                          yylval.val = atof(yytext); 
                          return NUMBER; 
                        }
{ID}                 { 
                          yylval.str = strdup(yytext); 
                          return IDENT; 
                        }
[ \t]               { /* Ignore whitespace */ }
\n                  { return '\n'; }
.                   { return yytext[0]; }
%%