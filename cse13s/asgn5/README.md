# Assignment 5

This program builds 4 different executable files that each serve a different purpose.

Encoder
>   Encoder takes in data from a file, stdin by default and encodes it using Hamming(8,4) encoding.
>   Output is then sent to a file, stdout by default.

Decoder
>   Decoder takes in data from a file, stdin by default and decodes it using Hamming(8,4) decoding.
>   Output is then sent to a file, stdout by default.

Error-Injection
>   This program injects errors via bitflips into input file, stdin by default. Output is sent to
>   file, stdout by default. Error rate is given by user, with 1% percent as the default. Seed given
>   by user, with 2021 as the default.

Entropy
>  This program will return the amount of entropy present in stdin, or from input file.
## Building

    $ make
    This will build encoder, decoder, error, and entropy
    $ make decode
    This will build decode.
    $ make encode
    This will build encode.
    $ make error
    This will build error.
    $ make entropy
    This will build entropy.

## Running

    $ ./encode -PARAMETERS

Acceptable parameters can be from 'hio'

-h will display a help message and then return.
-i FILE sets the input file to FILE, stdin by default.
-o FILE sets the output file to FILE, stdout by default.

    $ ./decode -PARAMETERS

Acceptable parameters can be from 'hiov'

-h will display a help message and then return.
-i FILE sets the input file to FILE, stdin by default.
-o FILE sets the output file to FILE, stdout by default.
-v will print statistics about the decoded file to stderr.

    $ ./error -PARAMETERS

Acceptable parameters can be from 'hes'

-h will display a help message and then return.
-e NUM will set the error rate to NUM, between 0.0 and 1.0., default .01
-s will set the seed, default of 2021

    $ ./entropy -PARAMETERS< FILE

Acceptable parameters can be from 'h'

-h will display a help message and then return.
This program will print the entropy of FILE to stdout.
If no FILE is given, will read from stdin.

## Cleaning

    $ make clean
