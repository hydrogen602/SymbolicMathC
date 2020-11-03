CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O -std=c11
LDLIBS = -lm
# add $(LDLIBS) for math

VALGRIND = -O0 -g

CFLAGS += $(VALGRIND)

HEADERS := $(wildcard *.h)

OTHER_OBJ_FILES := avltree.o

.PHONY: clean

all: $(HEADERS) $(OTHER_OBJ_FILES) main.o
	$(CC) -o main $(OTHER_OBJ_FILES) main.o $(LDLIBS)

test: clean $(HEADERS) $(OTHER_OBJ_FILES) test.o
	$(CC) -o test $(OTHER_OBJ_FILES) test.o $(LDLIBS)

clean:
	rm -f main test *.o 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c -o $@
