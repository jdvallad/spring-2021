CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
all: sorting

sorting: sorting.o
	$(CC) -o sorting sorting.o -lm

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c

clean:
	rm -f sorting
	rm -f *.o

scan-build: clean
	scan-build make

format: clean
	$(CC)-format -i -style=file *.h
	$(CC)-format -i -style=file *.c
