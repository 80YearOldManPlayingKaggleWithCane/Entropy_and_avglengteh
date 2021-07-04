#include "lzw.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>

#define BUFF_SIZE 1024

// Calc the average of bits per symbol for LZW.
double lzw_avg_len(FILE *fp, int lzw_width) {
    size_t n, tot = 0;
    unsigned char buff[BUFF_SIZE];
    std::string p;
    unsigned char _p;

    size_t entries = 1 << lzw_width;
    size_t index = 256; // first 256 are ASCII characters
    size_t output_cnt = 0;

    // The dict
    std::unordered_map<std::string, unsigned int> dict;

    // Read the very first character
    if (fread(&_p, 1, 1, fp) == 0) {
        return 0;
    }

    ++tot;
    p.push_back(_p);

    // Read BUFF_SIZE characters at a time
    while ((n = fread(buff, 1, BUFF_SIZE, fp)) > 0) {
        for (size_t i = 0; i < n; ++i) {
            // c = buff[i]

            // p = pc
            p.push_back(buff[i]);

            if (dict.count(p)) { // if pc in Dict
            } else {
                if (index < entries) {
                    dict[p] = index++; // add pc to Dict
                }

                // std::cout << p.substr(0, p.length() - 1) << std::endl;

                ++output_cnt; // output code p

                // p = c
                p.clear();
                p.push_back(buff[i]);
            }
        }
        tot += n;
    }

    // Deal with the last character(s)
    if (p.size()) {
        ++output_cnt;
    }

    return (double)output_cnt * lzw_width / tot; // return average length
}
