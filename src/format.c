#include <stdio.h>
#include <stdint.h>
#include "honey.h"

/*
 * Format HONEY amounts for Ledger UI
 *
 * - Input is always CONES (smallest unit)
 * - Signing uses full precision
 * - UI displays up to 6 decimals for readability
 *
 * Example:
 *   1234567000000000000 CONES
 *   → "1.234567 HNY"
 */

void format_hny_amount(
    uint64_t amount_cones,
    char *out,
    uint32_t out_len
) {
    uint64_t whole = amount_cones / CONES_PER_HNY;

    // Show only first 6 decimals (Ledger best practice)
    uint64_t fractional =
        (amount_cones / 1000000000000ULL) % 1000000ULL;

    snprintf(
        out,
        out_len,
        "%llu.%06llu HNY",
        whole,
        fractional
    );
}
