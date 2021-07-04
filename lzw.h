#ifndef __LZW_H__
#define __LZW_H__

#include <bits/types/FILE.h>

// Calc the average of bits per symbol for LZW.
double lzw_avg_len(FILE *fp, int lzw_width);

#endif // __LZW_H__
