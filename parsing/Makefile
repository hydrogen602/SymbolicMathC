CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O -std=c11
LDLIBS = -lm
# add $(LDLIBS) for math

VALGRIND = -O0 -g

CFLAGS += $(VALGRIND)

HEADERS := $(wildcard *.h)

.PHONY: build clean

build: lex.yy.o main.o arrayListToken.o
	gcc -o main lex.yy.o main.o arrayListToken.o

lex.yy.c: lexer.l
	flex lexer.l

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c -o $@

clean:
	rm -f *.o main lex.yy.c