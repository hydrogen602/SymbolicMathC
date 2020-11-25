CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O -std=c11
LDLIBS = -lm
# add $(LDLIBS) for math

VALGRIND = -O0 -g

CFLAGS += $(VALGRIND)

HEADERS := $(wildcard *.h)

OTHER_OBJ_FILES := array.o

LIB_OBJ_FILES := array.o str.o arrayListDefs.o

.PHONY: clean all lib

all: $(HEADERS) $(OTHER_OBJ_FILES) main.o
	$(CC) -o main $(OTHER_OBJ_FILES) main.o $(LDLIBS)

test: clean $(HEADERS) $(OTHER_OBJ_FILES) test.o arrayListDefs.o
	$(CC) -o test $(OTHER_OBJ_FILES) test.o arrayListDefs.o $(LDLIBS)

clean:
	rm -f main test *.o 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $*.c -o $@

lib: $(LIB_OBJ_FILES)
	ar -rc libH602Data.a *.o
	ranlib libH602Data.a