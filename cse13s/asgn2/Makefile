CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: mathlib-test

mathlib-test: mathlib-test.o
	$(CC) -o mathlib-test mathlib-test.o -lm

mathlib-test.o: mathlib-test.c
	$(CC) $(CFLAGS) -c mathlib-test.c

clean:
	rm -f mathlib-test mathlib-test.o

scan-build: clean
	scan-build make

syntax: clean
	$(CC)-format -i -style=file *.h
	$(CC)-format -i -style=file *.c
