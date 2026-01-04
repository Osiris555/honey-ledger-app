#include "handler_sign_tx.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/*
APDU DATA FORMAT:
[0..19]  recipient address (ASCII)
[20..27] uint64 amount (big-endian, 18 decimals)
*/

#define HONEY_DECIMALS 18
#define MAX_AMOUNT  ((uint64_t)-1)

/* ---------- VALIDATION ---------- */

static bool validate_amount(uint64_t amount) {
    if (amount == 0) {
        return false;
    }
    if (amount > MAX_AMOUNT) {
        return false;
    }
    return true;
}

/* ---------- HANDLER ---------- */

void handle_sign_tx(uint8_t *data, uint16_t len) {
    if (len != 28) {
        return; // malformed APDU
    }

    char address[65] = {0};
    char amount_str[32] = {0};

    /* Parse address */
    memcpy(address, data, 20);
    address[20] = '\0';

    /* Parse amount (uint64 BE) */
    uint64_t amount = 0;
    for (int i = 20; i < 28; i++) {
        amount = (amount << 8) | data[i];
    }

    /* Validate amount */
    if (!validate_amount(amount)) {
        return;
    }

    /* Convert to decimal string (18 decimals enforced) */
    uint64_t divisor = 1000000000000000000ULL;
    uint64_t whole   = amount / divisor;
    uint64_t frac    = amount % divisor;

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
