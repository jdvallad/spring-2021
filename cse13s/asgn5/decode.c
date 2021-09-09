#include "bm.c"
#include "hamming.c"

#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PARAMS "hvi:o:"

bool help = false;
bool verbose = false;
FILE *infile = NULL;
FILE *outfile = NULL;

// parses command line arguments
// sets the infile, the outfile and the help and verbose flags
void parse_arguments(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, PARAMS)) != -1) {
        switch (opt) {
        case 'h': help = true; return;
        case 'v': verbose = true; break;
        case 'i': infile = fopen(optarg, "rb"); break;
        case 'o': outfile = fopen(optarg, "wb"); break;
        default: break;
        }
    }
}

// sets the permissions of dst to those from src
// Code given in Assignment 5 PDF
void set_file_permissions(FILE *src, FILE *dst) {
    struct stat statbuf;
    fstat(fileno(src), &statbuf);
    fchmod(fileno(dst), statbuf.st_mode);
}
// prints the help message to stdout
void help_message() {
    printf("SYNOPSIS\n");
    printf("  A Hamming(8, 4) systematic code decoder.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./decode [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h             Program usage and help.\n");
    printf("  -v             Print statistics of decoding to stderr.\n");
    printf("  -i infile      Input data to decode.\n");
    printf("  -o outfile     Output of decoded data.\n");
    return;
}
// main driver for decoder
// parses the command line
// determines where to read the input and where to write output
// goes byte by byte through input, decoding each byte and writing to output
// keeps track of statistics for verbose
// if verbose in enabled, statistics are printed to stderr
// input and output files are closed and program exits
int main(int argc, char **argv) {
    parse_arguments(argc, argv);
    if (help) {
        help_message();
        return 0;
    }
    BitMatrix *Ht = bm_create_fill(
        8, 4, (char *[]) { "0111", "1011", "1101", "1110", "1000", "0100", "0010", "0001" });
    uint32_t total_bytes = 0, uncorrected_errors = 0, corrected_errors = 0;
    uint8_t read = 0, decoded_lower = 0, decoded_upper = 0, decoded_byte = 0;
    HAM_STATUS status;
    while (true) {
        read = fgetc(infile == NULL ? stdin : infile);
        total_bytes++;
        status = ham_decode(Ht, read, &decoded_lower);
        if (status == HAM_ERR) {
            uncorrected_errors++;
        }
        if (status == HAM_CORRECT) {
            corrected_errors++;
        }
        read = fgetc(infile == NULL ? stdin : infile);
        if (feof(infile == NULL ? stdin : infile)) {
            break;
        }
        total_bytes++;
        status = ham_decode(Ht, read, &decoded_upper);
        if (status == HAM_ERR) {
            uncorrected_errors++;
        }
        if (status == HAM_CORRECT) {
            corrected_errors++;
        }
        decoded_byte = pack_byte(decoded_upper, decoded_lower);
        fputc(decoded_byte, outfile == NULL ? stdout : outfile);
    }
    if (verbose) {
        fprintf(stderr, "Total bytes processed: %" PRIu32 "\n", total_bytes);
        fprintf(stderr, "Uncorrected errors: %" PRIu32 "\n", uncorrected_errors);
        fprintf(stderr, "Corrected errors: %" PRIu32 "\n", corrected_errors);
        fprintf(
            stderr, "Error rate: %1.6f\n", ((double) uncorrected_errors) / ((double) total_bytes));
    }
    set_file_permissions(infile == NULL ? stdin : infile, outfile == NULL ? stdout : outfile);
    if (infile != NULL) {
        fclose(infile);
    }
    if (outfile != NULL) {
        fclose(outfile);
    }
    bm_delete(&Ht);
    return 0;
}
