# Assignment 7

This program builds the executable file banhammer.

banhammer
>   Reads in input from stdin, and parses the input for oldspeak and badspeak.
>   Depending on the input, either a goodspeak, mixspeak, or badspeak message is
>   printed to stdout, along with the badspeak words and the oldspeak newspeak
>   pairs.

## Building

    $ make
    This will build banhammer.

## Running

    $ ./banhammer -PARAMETERS

Acceptable parameters can be from 'hsmt:f:'

-h will display a help message and then return.
-s will print statistics to stderr, supressing all other output and then return.
-m will enable the move-to-front rule.
-t SIZE will specify the hash table size (default is 10000).
-f SIZE will specify the Bloom filter size (default is 2^20).

## Cleaning

    $ make clean
