CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_GNU_SOURCE
LDFLAGS = -lm

all: compiler

compiler: compiler.o tokenizer.o parser.o codegen.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f compiler *.o output.asm output.o output