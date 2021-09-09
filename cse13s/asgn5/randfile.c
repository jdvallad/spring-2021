#include "bm.c"
#include "hamming.c"

#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PARAMS "hs:l:o:"

bool help = false;
unsigned int seed = 2021;
uint32_t length = 10;
FILE *outfile = NULL;

// parses command line arguments
// sets the infile, the outfile and the help flag
void parse_arguments(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, PARAMS)) != -1) {
        switch (opt) {
        case 'h': help = true; return;
        case 's': seed = atoi(optarg); break;
        case 'l': length = atoi(optarg); break;
        case 'o': outfile = fopen(optarg, "wb"); break;
        default: break;
        }
    }
}
// prints the help message to stdout
void help_message() {
    printf("SYNOPSIS\n");
    printf("  A random file generator.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./randfile [-h] [-s seed] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h             Program usage and help.\n");
    printf("  -s seed        sets the seed default 2021.\n");
    printf("  -l length      length of random output in bytes, default 10.\n");
    printf("  -o outfile     Output of random data, default stdout.\n");
    return;
}
int main(int argc, char **argv) {
    parse_arguments(argc, argv);
    if (help) {
        help_message();
        return 0;
    }
    srand(seed);
    for (uint32_t i = 0; i < length; i++) {
        fputc(((uint8_t) rand()), outfile == NULL ? stdout : outfile);
    }
    if (outfile != NULL) {
        fclose(outfile);
    }
    return 0;
}
