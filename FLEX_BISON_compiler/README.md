|
  Project- BY SWATANTRA PRATAP SINGH(23115903)
=
A simple compiler built using:
- Flex (for lexical analysis)
- Bison (for parsing)
- Gcc (for integration)
- It has built-in custom instruction for sigmoid function: sigmoid(<Expression)
- sigmoid(x) = 1 / (1 + e^(-x))

It parses a custom arithmatic equation and sigmoid function and prints the computed result just after parsing.

NO INTERMEDIATE CODE GENERATION OR CONVERSION TO ASSEMBLY OR BINARY OF THE EQUATION IS REQUIRED; JUST LEXING AND PARSING IS ENOUGH

------------------------------------------------------------
REQUIRED TOOLS
------------------------------------------------------------
Make sure you have the following installed:

- flex         → lexical analyzer generator
- bison        → parser generator
- gcc/g++      → compiler

Linux install (Debian/Ubuntu):

  sudo apt update
  
  sudo apt install flex bison gcc nasm make

Windows (manual):

    1. Search for the FLEX and BISON from the internet.    
    2. Don't forget to properly set up after installing.
    3. The provided YouTube link may be helpful(FOR ANY FURTHER ISSUE USE AI TO RESOLVE): https://youtu.be/8jCiE-TPV_Q?si=mhSd1OTgmSQy0RI8
    
------------------------------------------------------------
PROJECT STRUCTURE
------------------------------------------------------------

calc.l         - Flex definitions for tokens

calc.y        - Bison grammar and parser rules

input           - Dynamically from the terminal

------------------------------------------------------------
BUILDING THE COMPILER
------------------------------------------------------------

Step 1: Generate parser and lexer C files

    bison -d calc.y        → generates parser.tab.c, parser.tab.h
    flex calc.l            → generates lex.yy.c

Step 2: Compile everything

    gcc -o calc calc.tab.c lex.yy.c -lm       ->To link the parser and lexer together with gcc

Step 3: Run the compiler

    ./calc        (LINUX)
    calc.exe      (WINDOWS)

------------------------------------------------------------
NOTES
------------------------------------------------------------

- Tokens in calc.l should match %token in calc.y
- All rules in calc.y must end with semicolons
- Ensure return values in lexer are correct
- You can test different input programs as it takes dynamic inputs
