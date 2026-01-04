#pragma once

#include <stdint.h>

/*
 * Convert uint64 amount (18 decimals) into human-readable string.
 *
 * Example:
 *   1230000000000000000 -> "1.23 HNY"
 */
void format_honey_amount(
    uint64_t amount,
    char *out,
    unsigned int out_len
);
