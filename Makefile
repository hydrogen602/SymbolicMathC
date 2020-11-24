CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O -std=c11
LDLIBS = -lm
# add $(LDLIBS) for math

VALGRIND = -O0 -g

CFLAGS += $(VALGRIND)

HEADERS := $(wildcard *.h)
HEADERS += $(wildcard dataTypes/*.h)
HEADERS += $(wildcard dataTypes/mathobj/*.h)
HEADERS += $(wildcard dataTypes/mathobj/types/*.h)
HEADERS += $(wildcard dataStructs/*.h)


DATA_TYPES := $(patsubst %.c,%.o,$(wildcard dataTypes/*.c))
DATA_TYPES += $(patsubst %.c,%.o,$(wildcard dataTypes/mathobj/*.c))
DATA_TYPES += $(patsubst %.c,%.o,$(wildcard dataTypes/mathobj/types/*.c))
DATA_TYPES += $(patsubst %.c,%.o,$(filter-out dataStructs/main.c dataStructs/test.c, $(wildcard dataStructs/*.c)))
DATA_TYPES += parsing/parser.tab.o parsing/lex.yy.o parsing/parse.o


OTHER_OBJ_FILES := errors.o #parser.o parser2.o

.PHONY: clean all test parseTest parse

all: parse $(DATA_TYPES) $(HEADERS) $(OTHER_OBJ_FILES) main.o
	$(CC) -o main $(DATA_TYPES) $(OTHER_OBJ_FILES) main.o $(LDLIBS)

parse:
	$(MAKE) -C parsing build

test: clean $(DATA_TYPES) $(HEADERS) $(OTHER_OBJ_FILES) test.o
	$(CC) -o test $(DATA_TYPES) $(OTHER_OBJ_FILES) test.o $(LDLIBS)

# parseTest: clean $(HEADERS) $(OTHER_OBJ_FILES) $(DATA_TYPES) parseTest.o
# 	$(CC) -o parseTest $(DATA_TYPES) $(OTHER_OBJ_FILES) parseTest.o $(LDLIBS)


clean:
	rm -f main test *.o dataTypes/*.o dataTypes/mathobj/*.o dataTypes/mathobj/types/*.o dataStructs/*.o
	$(MAKE) -C parsing clean

# LL\(1\)TableGen.py:
# 	python3 LL\(1\)TableGen.py

# parseTable.o: parseTable.c $(HEADERS) LL\(1\)TableGen.py
# 	$(CC) $(CFLAGS) -c parseTable.c -o parseTable.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c -o $@

dataTypes/%.o: dataTypes/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c dataTypes/$*.c -o $@

dataTypes/mathobj/%.o: dataTypes/mathobj/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c dataTypes/mathobj/$*.c -o $@

dataTypes/mathobj/%.o: dataTypes/mathobj/types/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c dataTypes/mathobj/types/$*.c -o $@

dataStructs/%.o: dataStructs/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c dataStructs/$*.c -o $@

parsing/%.o: parsing/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c parsing/$*.c -o $@