#pragma once

#include <stdint.h>

void format_hny_amount(
    uint64_t value,
    char *out,
    uint32_t out_len
);

void format_address(
    const uint8_t *addr,
    char *out,
    uint32_t out_len
);
