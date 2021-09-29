#include "code.c"
#include "header.h"
#include "huffman.c"
#include "io.c"

#include <stdio.h>
#include <unistd.h>
#define PARAMS "hvi:o:"
#include <sys/stat.h>
#include <sys/types.h>

// flags to track whether to print help message or verbose statistics
bool help = false;
bool verbose = false;
// default infile (0 or stdin)
// default outfile (1 or stdout)
int infile = 0;
int outfile = 1;
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
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT); break;
        case 'v': verbose = true; break;
        default: break;
        }
    }
}
// prints the help message to stdout
void help_message() {
    printf("SYNOPSIS\n");
    printf("  A Huffman decoder.\n");
    printf("  Decompresses a file using the Huffman coding algorithm.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./decode [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h             Program usage and help.\n");
    printf("  -v             Print compression statistics.\n");
    printf("  -i infile      Input data to decompress.\n");
    printf("  -o outfile     Output of decompressed data.\n");
    return;
}

// Uses remaining bits in file to traverse huffman tree and reconstruct the compressed file.
// Reconstructed file printed to outfile.
void decompress_file(Node *tree, uint64_t file_size) {
    Node *n = tree;
    uint8_t bit = 0;
    uint64_t count = 0;
    while (count != file_size) {
        read_bit(infile, &bit);
        if (bit) {
            n = n->right;
        } else {
            n = n->left;
        }
        if (node_is_leaf(n)) {
            bit = n->symbol;
            write_bytes(outfile, &bit, 1);
            count++;
            n = tree;
        }
    }
    return;
}

// Main function.
// Parses command line arguments, then prints the help message if necessary.
// Reconstructs the header from infile.
// If magic number from header does not match MAGIC (defined in defines.h), then print error
// message and return.
// Then sets permission of outfile to same as infile.
// Reconstruct the tree_dump from infile.
// Reconstruct huffman tree from tree_dump.
// Then iterate over rest of bits in infile, using huffman tree to decompress the file.
// If verbose flag, print statistics.
// Garbage collection, then exit.
int main(int argc, char **argv) {
    parse_arguments(argc, argv);
    if (help) {
        help_message();
        return 0;
    }
    struct Header header;
    read_bytes(infile, (uint8_t *) &header, sizeof(Header));
    if (header.magic != MAGIC) {
        printf("Invalid magic number.\n");
        return 1;
    }
    fchmod(outfile, header.permissions);
    uint8_t tree_dump[header.tree_size];
    read_bytes(infile, tree_dump, header.tree_size);
    Node *tree = rebuild_tree(header.tree_size, tree_dump);
    decompress_file(tree, header.file_size);
    if (verbose) {
        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_read);
        fprintf(stderr, "Decompressed file size: %" PRIu64 " bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %2.2f%%\n",
            100. * ((float) (bytes_written - bytes_read)) / ((float) bytes_written));
    }
    delete_tree(&tree);
    close(infile);
    close(outfile);
    return 0;
}
