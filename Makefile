CC = gcc
CFLAGS = -Wall -pedantic -O -std=c11
LDLIBS = -lm
# add $(LDLIBS) for math

VALGRIND = -O0 -g

CFLAGS += $(VALGRIND)

HEADERS := $(wildcard *.h)
HEADERS += $(wildcard dataTypes/*.h)
HEADERS += $(wildcard dataTypes/mathobj/*.h)

DATA_TYPES := $(patsubst %.c,%.o,$(wildcard dataTypes/*.c))
DATA_TYPES += $(patsubst %.c,%.o,$(wildcard dataTypes/mathobj/*.c))


OTHER_OBJ_FILES := parser.o errors.o

.PHONY: clean

all: $(DATA_TYPES) $(HEADERS) $(OTHER_OBJ_FILES) main.o
	$(CC) -o main $(DATA_TYPES) $(OTHER_OBJ_FILES) main.o $(LDLIBS)

test: clean $(DATA_TYPES) $(HEADERS) $(OTHER_OBJ_FILES) test.o
	$(CC) -o test $(DATA_TYPES) $(OTHER_OBJ_FILES) test.o $(LDLIBS)

clean:
	rm -f main test *.o dataTypes/*.o dataTypes/mathobj/*.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c -o $@

dataTypes/%.o: dataTypes/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c dataTypes/$*.c -o $@

dataTypes/mathobj/%.o: dataTypes/mathobj/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c dataTypes/mathobj/$*.c -o $@