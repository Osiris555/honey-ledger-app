#pragma once

#include <stdint.h>
#include <stddef.h>

void handle_apdu(
    uint8_t cla,
    uint8_t ins,
    uint8_t p1,
    uint8_t p2,
    uint8_t lc,
    const uint8_t *data,
    uint8_t *tx,
    size_t *tx_len,
    uint16_t *sw
);
