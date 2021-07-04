#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <cstddef>
#include <iostream>

struct HuffmanFreq {
    // char c;
    unsigned int freq;

    // Compare two HuffmanFreq(s) by their freq.
    bool operator<(const HuffmanFreq &rhs) const;
};

// Count frequencies of characters in the file.
// Return number of valid characters whose freqency is not zero.
// Total number of characters in the file will be store in 'tot'.
size_t count_freq(FILE *fp, HuffmanFreq freq[256], size_t *tot);

// Calc the Shannon's entropy.
double shannon_entropy(const HuffmanFreq *freq, size_t n, size_t tot);

// Calc the average of bits per symbol for static Huffman.
double huffman_avg_len(const HuffmanFreq *freq, size_t n, size_t tot);

#endif // __HUFFMAN_H__
