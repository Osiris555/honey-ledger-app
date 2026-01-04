#include "ui.h"
#include <stdio.h>

#define HONEY_DECIMALS 18
#define HONEY_SYMBOL   " HNY"

/*
 * Formats a uint64 amount assuming 18 decimals.
 * Uses up to 6 decimal places for display.
 */
void format_honey_amount(
    uint64_t amount,
    char *out,
    unsigned int out_len
) {
    uint64_t divisor = 1000000000000000000ULL;

    uint64_t whole = amount / divisor;
    uint64_t frac  = amount % divisor;

    /* Reduce fraction to 6 decimals for display */
    frac /= 1000000000000ULL;

    if (frac == 0) {
        snprintf(out, out_len, "%llu%s",
            (unsigned long long)whole,
            HONEY_SYMBOL
        );
    } else {
        snprintf(out, out_len, "%llu.%06llu%s",
            (unsigned long long)whole,
            (unsigned long long)frac,
            HONEY_SYMBOL
        );
    }
}
