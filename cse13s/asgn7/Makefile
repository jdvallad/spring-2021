CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
all: banhammer

banhammer:
	$(CC) $(CFLAGS) -o banhammer banhammer.c -lm


clean:
	rm -f banhammer
	rm -f *.o

scan-build: clean
	scan-build make

format: clean
	$(CC)-format -i -style=file *.h
	$(CC)-format -i -style=file *.c
