#include "format.h"
#include <stdio.h>
#include <string.h>

#define HNY_DECIMALS 6

void format_hny_amount(
    uint64_t value,
    char *out,
    uint32_t out_len
) {
    uint64_t integer = value;
    uint64_t fraction = 0;

    for (int i = 0; i < HNY_DECIMALS; i++) {
        fraction = integer % 10;
        integer /= 10;
    }

    uint64_t frac_part = value % 1000000;
    uint64_t int_part  = value / 1000000;

    snprintf(
        out,
        out_len,
        "%llu.%06llu HNY",
        int_part,
        frac_part
    );
}

/*
 * Displays:
 * hny1abcd…1234
 */
void format_address(
    const uint8_t *addr,
    char *out,
    uint32_t out_len
) {
    snprintf(
        out,
        out_len,
        "hny1%02X%02X…%02X%02X",
        addr[0],
        addr[1],
        addr[30],
        addr[31]
    );
}
