#include "code.c"
#include "header.h"
#include "huffman.c"
#include "io.c"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PARAMS              "hvi:o:"
#define MAX_FILEPATH_LENGTH 300

// flags to track whether to print help message or verbose statistics
bool help = false;
bool verbose = false;
// default infile (0 or stdin)
// default outfile (1 or stdout)
int infile = 0;
int outfile = 1;
// stores filepath for input file, used to reopen file to reiterate over bytes
char input_path[MAX_FILEPATH_LENGTH];
// variables to track statistics
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

// parses command line arguments
// sets the infile, the outfile and the help and verbose flags
void parse_arguments(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, PARAMS)) != -1) {
        switch (opt) {
        case 'h': help = true; return;
        case 'i':
            infile = open(optarg, O_RDONLY);
            strcpy(input_path, optarg);
            break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT); break;
        case 'v': verbose = true; break;
        default: break;
        }
    }
}
// prints the help message to stdout
void help_message() {
    printf("SYNOPSIS\n");
    printf("  A Huffman encoder.\n");
    printf("  Compresses a file using the Huffman coding algorithm.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./encode [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h             Program usage and help.\n");
    printf("  -v             Print compression statistics.\n");
    printf("  -i infile      Input data to compress.\n");
    printf("  -o outfile     Output of compressed data.\n");
    return;
}
// Recursively iterates over the huffman tree represented by n and writes each node to outfile
// Used in decode to reconstruct the huffman tree.
void write_tree_dump(Node *n, int outfile) {
    if (n == NULL) {
        return;
    }
    write_tree_dump(n->left, outfile);
    write_tree_dump(n->right, outfile);
    uint8_t c;
    if (node_is_leaf(n)) {
        c = 'L';
        write_bytes(outfile, &c, 1);
        c = n->symbol;
        write_bytes(outfile, &c, 1);
    } else {
        c = 'I';
        write_bytes(outfile, &c, 1);
    }
    return;
}

// Iterates over input file in order to construct the histogram of all the characters in the file.
void construct_histogram(uint64_t hist[ALPHABET]) {
    for (int i = 0; i < ALPHABET; i++) {
        hist[i] = 0;
    }
    hist[0]++;
    hist[ALPHABET - 1]++;
    uint8_t buffer = 0;
    while (read_bytes(infile, &buffer, 1)) {
        hist[buffer]++;
    }
    return;
}

// creates the header for the compressed file, setting all the appropriate variables.
void construct_header(struct Header *header, uint64_t hist[ALPHABET]) {
    header->magic = MAGIC;
    struct stat statbuf;
    fstat(infile, &statbuf);
    fchmod(outfile, statbuf.st_mode);
    header->permissions = statbuf.st_mode;
    uint16_t tree_size = 0;
    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            tree_size++;
        }
    }
    header->tree_size = (3 * tree_size) - 1;
    header->file_size = statbuf.st_size;
    return;
}
// iterates over the infile, writing the code for each character in infile to outfile.
// Will only write the first file_size bytes from infile to outfile.
void compress_file(Code table[ALPHABET], unsigned long file_size) {
    bytes_read = 0;
    uint8_t buffer;
    unsigned long size = 0;
    while (size < file_size) {
        read_bytes(infile, &buffer, 1);
        write_code(outfile, &table[buffer]);
        size++;
    }
}

// Main function.
// Parses command line arguments, printing help message and exiting if necessary.
// Constructs histogram from infile.
// Builds huffman tree from this histogram.
// Creates the Code Table by iterating over the huffman tree.
// Constructs the header file for the encoded file.
// writes the header file to outfile
// creates and writes the huffman tree as a tree dump to outfile.
// close and reopen infile (so that it can be reiterated over).
// Using code table, write each character from infile to outfile using it's respective code.
// Flush codes left in buffer to outfile.
// print verbose statistics if necessary.
// Garbage collection and then return.
int main(int argc, char **argv) {
    parse_arguments(argc, argv);
    if (help) {
        help_message();
        return 0;
    }
    uint64_t hist[ALPHABET];
    Code table[ALPHABET];
    construct_histogram(hist);
    Node *n = build_tree(hist);
    build_codes(n, table);
    struct Header header;
    construct_header(&header, hist);
    write_bytes(outfile, (uint8_t *) &header, sizeof(Header));
    write_tree_dump(n, outfile);
    close(infile);
    infile = open(input_path, O_RDONLY);
    compress_file(table, header.file_size);
    flush_codes(outfile);
    if (verbose) {
        fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n", bytes_read);
        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %2.2f%%\n",
            100. * ((float) (bytes_read - bytes_written)) / ((float) bytes_read));
    }
    delete_tree(&n);
    close(infile);
    close(outfile);
    return 0;
}
