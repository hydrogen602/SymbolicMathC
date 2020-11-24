CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O -std=c11
LDLIBS = -lm
# add $(LDLIBS) for math

VALGRIND = -O0 -g

CFLAGS += $(VALGRIND)

HEADERS := $(wildcard *.h)

OTHER_FILES := ../dataStructs/str.o ../dataStructs/array.o

OBJ := parser.tab.o lex.yy.o main.o arrayListToken.o tokens.o

.PHONY: build clean

build: $(OBJ) $(OTHER_FILES)
	gcc -o main $(OBJ) $(OTHER_FILES)

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.c: parser.y
	bison -d parser.y

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c -o $@

clean:
	rm -f *.o main lex.yy.c