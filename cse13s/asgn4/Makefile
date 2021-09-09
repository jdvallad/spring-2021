CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
all: tsp

tsp: tsp.o
	$(CC) -o tsp tsp.o -lm

tsp.o: tsp.c
	$(CC) $(CFLAGS) -c tsp.c

clean:
	rm -f tsp
	rm -f *.o

scan-build: clean
	scan-build make

format: clean
	$(CC)-format -i -style=file *.h
	$(CC)-format -i -style=file *.c
