#include "bm.c"
#include "hamming.c"

#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PARAMS "hi:o:"

bool help = false;
FILE *infile = NULL;
FILE *outfile = NULL;

// parses command line arguments
// sets the infile, the outfile and the help flag
void parse_arguments(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, PARAMS)) != -1) {
        switch (opt) {
        case 'h': help = true; return;
        case 'i': infile = fopen(optarg, "rb"); break;
        case 'o': outfile = fopen(optarg, "wb"); break;
        default: break;
        }
    }
}
// prints the help message to stdout
void help_message() {
    printf("SYNOPSIS\n");
    printf("  A Hamming(8, 4) systematic code generator.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./encode [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h             Program usage and help.\n");
    printf("  -i infile      Input data to encode.\n");
    printf("  -o outfile     Output of encoded data.\n");
    return;
}
// sets the permissions of dst to those from src
// Code given in Assignment 5 PDF
void set_file_permissions(FILE *src, FILE *dst) {
    struct stat statbuf;
    fstat(fileno(src), &statbuf);
    fchmod(fileno(dst), statbuf.st_mode);
}
// main driver for encoder
// parses the command line
// determines where to read the input and where to write output
// goes byte by byte through input, encoding each byte and writing to output
// keeps track of statistics for verbose
// if verbose in enabled, statistics are printed to stderr
// input and output files are closed and program exits
int main(int argc, char **argv) {
    parse_arguments(argc, argv);
    if (help) {
        help_message();
        return 0;
    }
    BitMatrix *H
        = bm_create_fill(4, 8, (char *[]) { "01111000", "10110100", "11010010", "11100001" });
    uint8_t read = 0, encoded_lower = 0, encoded_upper = 0;
    while (true) {
        read = fgetc(infile == NULL ? stdin : infile);
        if (feof(infile == NULL ? stdin : infile)) {
            break;
        }
        encoded_lower = ham_encode(H, pack_byte(0, lower_nibble(read)));
        encoded_upper = ham_encode(H, pack_byte(0, upper_nibble(read)));
        fputc(encoded_lower, outfile == NULL ? stdout : outfile);
        fputc(encoded_upper, outfile == NULL ? stdout : outfile);
    }
    set_file_permissions(infile == NULL ? stdin : infile, outfile == NULL ? stdout : outfile);
    if (infile != NULL) {
        fclose(infile);
    }
    if (outfile != NULL) {
        fclose(outfile);
    }
    bm_delete(&H);
    return 0;
}
