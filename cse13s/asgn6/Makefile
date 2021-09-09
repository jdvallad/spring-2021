CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
all: encode decode entropy

encode:
	$(CC) $(CFLAGS) -o encode encode.c -lm

decode:
	$(CC) $(CFLAGS) -o decode decode.c -lm

entropy:
	$(CC) $(CFLAGS) -o entropy entropy.c -lm

clean:
	rm -f encode
	rm -f decode
	rm -f entropy
	rm -f *.o

scan-build: clean
	scan-build make

format: clean
	$(CC)-format -i -style=file *.h
	$(CC)-format -i -style=file *.c
