#include "bf.c"
#include "ht.c"
#include "messages.h"
#include "parser.c"

#include <inttypes.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Parameters used in command line.
#define PARAMS "ht:f:ms"
// Regular Expression that defines a word.
#define WORD "[a-zA-Z0-9_'-]+"

// Flags set in command line.
uint64_t hash_size = 10000;
uint64_t bloom_size = 0x100000;
bool help = false;
bool move_to_front = false;
bool statistics = false;

// Variables to keep track of statistics.
uint64_t seeks = 0;
uint64_t links = 0;

// Parses Command Line and sets flags accordingly.
void parse_arguments(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, PARAMS)) != -1) {
        switch (opt) {
        case 'h': help = true; return;
        case 't': hash_size = strtol(optarg, NULL, 10); break;
        case 'f': bloom_size = strtol(optarg, NULL, 10); break;
        case 'm': move_to_front = true; break;
        case 's': statistics = true; break;
        default: break;
        }
    }
}

// Prints help message to stderr.
void help_message() {
    printf("SYNOPSIS\n");
    printf("  A word filtering program for the GPRSC.\n");
    printf("  Filters out and reports bad words parsed from stdin.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./banhammer [-hsm] [-t size] [-f size]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h           Program usage and help.\n");
    printf("  -s           Print program statistics.\n");
    printf("  -m           Enable move-to-front rule.\n");
    printf("  -t size      Specify hash table size (default: 10000).\n");
    printf("  -f size      Specify Bloom filter size (default: 2^20).\n");
    return;
}

// Main function that is run.
// First, parses command line and prints help message and returns, if necessary.
// Then created the BloomFilter, HashTable, and the Regular Expression.
// The BloomFilter and the HashTable are then populated from badspeak.txt and
// newspeak.txt.
// 2 LinkedLists are then created to keep track of every oldspeak and badspeak
// word that is found in stdin.
// stdin is then iterated over, with every word being checked against the
// BloomFilter and the HashTable, if necessary. Oldspeak or badspeak words that
// are found are added to the respective LinkedList. Finally, if the statistics
// flag has not been set, a corresponding message is printed to stdout, along
// with the contents of the LinkedLists for oldspeak and badspeak.
// Otherwise, statistics are printed to stderr.
// Garbage Collection is then performed, and the function exits.
int main(int argc, char **argv) {
    parse_arguments(argc, argv);
    if (help) {
        help_message();
        return 0;
    }
    BloomFilter *bf = bf_create(bloom_size);
    HashTable *ht = ht_create(hash_size, move_to_front);
    regex_t re;
    // Code used from Assignment PDF
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        bf_delete(&bf);
        ht_delete(&ht);
        regfree(&re);
        return 1;
    }
    char *word = NULL;
    FILE *badspeak_file = fopen("badspeak.txt", "r");
    FILE *newspeak_file = fopen("newspeak.txt", "r");
    while ((word = next_word(badspeak_file, &re)) != NULL) {
        bf_insert(bf, word);
        ht_insert(ht, word, NULL);
    }
    clear_words();
    fclose(badspeak_file);
    while ((word = next_word(newspeak_file, &re)) != NULL) {
        bf_insert(bf, word);
        char *temp = string_duplicate(word);
        word = next_word(newspeak_file, &re);
        ht_insert(ht, temp, word);
        free(temp);
        temp = NULL;
    }
    clear_words();
    fclose(newspeak_file);
    LinkedList *bad = ll_create(false);
    LinkedList *new = ll_create(false);
    Node *n = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        if (bf_probe(bf, word)) {
            if ((n = ht_lookup(ht, word)) != NULL) {
                if (n->newspeak) {
                    ll_insert(new, n->oldspeak, n->newspeak);
                } else {
                    ll_insert(bad, n->oldspeak, NULL);
                }
            }
        }
    }
    clear_words();
    bool bad_bool = ll_length(bad) > 0;
    bool new_bool = ll_length(new) > 0;
    if (!statistics) {
        if (bad_bool && new_bool) {
            printf("%s", mixspeak_message);
            ll_print(bad);
            ll_print(new);
        }
        if (bad_bool && !new_bool) {
            printf("%s", badspeak_message);
            ll_print(bad);
        }
        if (!bad_bool && new_bool) {
            printf("%s", goodspeak_message);
            ll_print(new);
        }
    } else {
        float asl = ((float) links) / ((float) seeks);
        float htl = 100. * ((float) ht_count(ht)) / ((float) ht_size(ht));
        float bfl = 100. * ((float) bf_count(bf)) / ((float) bf_size(bf));
        fprintf(stderr, "Seeks: %lu\n", seeks);
        fprintf(stderr, "Average seek length: %2.6f\n", asl);
        fprintf(stderr, "Hash table load: %2.6f%%\n", htl);
        fprintf(stderr, "Bloom filter load: %2.6f%%\n", bfl);
    }
    bf_delete(&bf);
    ht_delete(&ht);
    regfree(&re);
    ll_delete(&bad);
    ll_delete(&new);
    return 0;
}
