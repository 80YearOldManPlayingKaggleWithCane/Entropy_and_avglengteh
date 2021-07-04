#include "huffman.h"
#include "lzw.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

// Default the fixed width of codes for LZW.
#define DEFAULT_LZW_WIDTH 12

// Print usage info and exit the program with -1.
void usage(char *it) {
    fprintf(stderr, "Usage: %s [-s LZW WIDTH] <inputfile>\n", it);
    exit(-1);
}

// Parse command line arguments.
int parse_argv(int argc, char **argv, char *&opt_s, char *&path) {
    int i;
    opt_s = NULL;
    path = NULL;
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-s") == 0) { // the optional argument "-s"
            ++i;
            if (i < argc) {
                opt_s = argv[i];
            } else {
                fprintf(stderr, "Missing argument!\n");
                usage(argv[0]);
            }
        } else { // the path to the given file to be compressed
            if (path == NULL) {
                path = argv[i];
            } else {
                fprintf(stderr, "Unknown argument!\n");
                usage(argv[0]);
            }
        }
    }
    return i;
}

int main(int argc, char **argv) {
    int lzw_width = DEFAULT_LZW_WIDTH;

    char *opt_s_str;
    char *path;

    // Parse command line arguments
    parse_argv(argc, argv, opt_s_str, path);

    // Check command line arguments
    if (opt_s_str) {
        lzw_width = atoi(opt_s_str);
    }
    if (!(lzw_width >= 9 && lzw_width <= 18)) {
        fprintf(stderr, "LZW WIDTH out of range [9-18]!\n");
        usage(argv[0]);
    }

    // Open input file
    FILE *fp = fopen(path, "r"); // read-only!

    if (fp == NULL) {
        perror("fopen");
        exit(-1);
    }

    // Read file and build data for first two numbers:
    // The Shannon's entropy and Huffman Average Length

    HuffmanFreq freq[256];

    size_t tot;
    size_t n = count_freq(fp, freq, &tot);
    rewind(fp);

    // Read file and calculate the third number:
    // LZW average length
    double lzw_avglen = lzw_avg_len(fp, lzw_width);

    // Close input file
    fclose(fp);

    // Calculate the first two numbers
    double entropy = shannon_entropy(freq, n, tot);
    double huffman_avglen = huffman_avg_len(freq, n, tot);

    // Output all three numbers
    printf("entropy:%.2lf\n", entropy);
    printf("huffman_avglen:%.2lf\n", huffman_avglen);
    printf("lzw_avglen:%.2lf\n", lzw_avglen);

    return 0;
}
