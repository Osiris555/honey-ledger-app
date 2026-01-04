#include "handler_sign_tx.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>

/*
APDU DATA FORMAT (for now):
[0..19]  recipient address (ASCII, null-terminated if shorter)
[20..31] amount uint64 (big-endian, 18 decimals implied)
*/

void handle_sign_tx(uint8_t *data, uint16_t len) {
    char address[65] = {0};
    char amount_str[32] = {0};

    if (len < 32) {
        return;
    }

    /* Parse address */
    memcpy(address, data, 20);
    address[20] = '\0';

    /* Parse amount */
    uint64_t amount = 0;
    for (int i = 20; i < 28; i++) {
        amount = (amount << 8) | data[i];
    }

    /* Convert amount to decimal string (18 decimals) */
    uint64_t whole = amount / 1000000000000000000ULL;
    uint64_t frac  = amount % 1000000000000000000ULL;

    snprintf(
        amount_str,
        sizeof(amount_str),
        "%llu.%018llu",
        (unsigned long long)whole,
        (unsigned long long)frac
    );

    ui_init();
    ui_display_tx(address, amount_str);
}
