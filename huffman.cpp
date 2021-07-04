#include "huffman.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>

#define BUFF_SIZE 1024

// The HuffmanNode struct.
// Note that since we do not need to actually do the encrypt processing, so
// don't need to build the real huffman tree.
struct HuffmanNode {
    unsigned int parent;
    unsigned int freq;
};

// HuffmanNode comparator
struct HuffmanNodeCmp {
    // Compare two HuffmanNode pointers by their freq.
    bool operator()(const HuffmanNode *n1, const HuffmanNode *n2) const {
        return n2->freq < n1->freq;
    }
};

// Compare two HuffmanFreq(s) by their freq.
bool HuffmanFreq::operator<(const HuffmanFreq &rhs) const {
    return rhs.freq < freq;
}

// Count frequencies of characters in the file.
// Return number of valid characters whose freqency is not zero.
// Total number of characters in the file will be store in 'tot'.
size_t count_freq(FILE *fp, HuffmanFreq freq[256], size_t *tot) {
    unsigned char buff[BUFF_SIZE];
    size_t n;
    size_t tot_ = 0;

    // Init
    for (int i = 0; i < 256; ++i) {
        freq[i].freq = 0;
    }

    // Read BUFF_SIZE characters at a time
    while ((n = fread(buff, 1, BUFF_SIZE, fp)) > 0) {
        // Count these characters
        for (size_t i = 0; i < n; ++i) {
            // assert((unsigned int)buff[i] < 256);
            ++freq[(unsigned int)buff[i]].freq;
        }
        tot_ += n;
    }

    // Sort the frequencies array
    std::sort(freq, freq + 256);

    // Count number of valid characters whose freq>0
    n = 0;
    while (n < 256 && freq[n].freq) {
        ++n;
    }

    *tot = tot_;

    return n; // return number of valid characters
}

// Calc the Shannon's entropy.
double shannon_entropy(const HuffmanFreq *freq, size_t n, size_t tot) {
    double entropy = 0;
    for (size_t i = 0; i < n; ++i) {
        double possibility = (double)freq[i].freq / tot;
        entropy += -possibility * log2(possibility);
    }
    return entropy;
}

// Calc the average of bits per symbol for static Huffman.
double huffman_avg_len(const HuffmanFreq *freq, size_t n, size_t tot) {
    HuffmanNode *nodes = new HuffmanNode[n * 2];

    // A min heap
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>,
                        HuffmanNodeCmp>
        q;

    // Init
    for (size_t i = 0; i < n; ++i) {
        nodes[i].freq = freq[i].freq;
        nodes[i].parent = i;
        q.push(&nodes[i]);
    }

    size_t idx = n;

    // Build the tree from bottom to top
    while (q.size() >= 2) {
        // Pop two smallest nodes
        HuffmanNode *n1 = q.top();
        q.pop();
        HuffmanNode *n2 = q.top();
        q.pop();

        n1->parent = idx;
        n2->parent = idx;

        // Merge them into a new node
        nodes[idx].freq = n1->freq + n2->freq;
        nodes[idx].parent = idx;

        // Push the new node to the heap
        q.push(&nodes[idx]);
        ++idx;
    }

    // Count total length in bits after compression
    size_t tot_len = 0;
    for (size_t i = 0; i < n; ++i) {
        // Wind from bottom to top of the tree, count the depth of current node
        // The depth is the length in bits to represent the symbol
        unsigned int len = 0;
        unsigned int idx = i;
        while (nodes[idx].parent != idx) {
            idx = nodes[idx].parent;
            ++len;
        }

        tot_len += nodes[i].freq * len;
    }

    delete[] nodes;

    return (double)tot_len / tot; // return average length
}
