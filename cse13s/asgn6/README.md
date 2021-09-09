# Assignment 6

This program builds 3 different executable files that each serve a different purpose.

Encoder
>   Encoder takes in data from a file, stdin by default and compresses it using Huffman coding.
>   Output is then sent to a file, stdout by default.

Decoder
>   Decoder takes in data from a file, stdin by default and decompresses it using Huffman coding.
>   Output is then sent to a file, stdout by default.

Entropy
>  This program will return the amount of entropy present in stdin, or from input file.
## Building

    $ make
    This will build encoder, decoder, and entropy.
    $ make decode
    This will build decode.
    $ make encode
    This will build encode.
    $ make entropy
    This will build entropy.

## Running

    $ ./encode -PARAMETERS

Acceptable parameters can be from 'hvi:o:'

-h will display a help message and then return.
-v will print compression statistics to stderr and then return.
-i FILE sets the input file to FILE, stdin by default.
-o FILE sets the output file to FILE, stdout by default.

    $ ./decode -PARAMETERS

Acceptable parameters can be from 'hvi:o:'

-h will display a help message and then return.
-v will print compression statistics to stderr and then return.
-i FILE sets the input file to FILE, stdin by default.
-o FILE sets the output file to FILE, stdout by default.

    $ ./entropy -PARAMETERS< FILE

Acceptable parameters can be from 'h'

-h will display a help message and then return.
This program will print the entropy of FILE to stdout.
If no FILE is given, will read from stdin.

## Cleaning

    $ make clean
