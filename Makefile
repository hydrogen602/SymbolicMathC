CC = gcc
CFLAGS = -Wall -pedantic -O -std=c99
LDLIBS = -lm
# add $(LDLIBS) for math

VALGRIND = -O0 -g

CFLAGS += $(VALGRIND)

HEADERS := $(wildcard *.h)
HEADERS += $(wildcard dataTypes/*.h)

DATA_TYPES := $(patsubst %.c,%.o,$(wildcard dataTypes/*.c))

.PHONY: clean

all: $(DATA_TYPES) main.o
	$(CC) -o main $(DATA_TYPES) main.o $(LDLIBS)

clean:
	rm -f main *.o dataTypes/*.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c -o $@

dataTypes/%.o: dataTypes/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c dataTypes/$*.c -o $@