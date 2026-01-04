#include "handler_sign_tx.h"
#include "ui.h"

#include <string.h>
#include <stdio.h>

#define P1_FIRST 0x00
#define P1_MORE  0x01
#define P1_LAST  0x02

static tx_context_t tx_ctx;

/*
FINAL TX FORMAT (after reassembly):
[0..19]  recipient address (ASCII)
[20..27] uint64 amount (18 decimals, BE)
*/

static bool validate_amount(uint64_t amount) {
    if (amount == 0) return false;
    return true;
}

void handle_sign_tx_chunk(uint8_t p1, uint8_t *data, uint16_t len) {
    if (p1 == P1_FIRST) {
        memset(&tx_ctx, 0, sizeof(tx_ctx));
        tx_ctx.active = true;
    }

    if (!tx_ctx.active) {
        return;
    }

    if (tx_ctx.length + len > MAX_TX_SIZE) {
        tx_ctx.active = false;
        return;
    }

    memcpy(tx_ctx.buffer + tx_ctx.length, data, len);
    tx_ctx.length += len;

    if (p1 != P1_LAST) {
        return;
    }

    /* ----- FINAL CHUNK RECEIVED ----- */

    if (tx_ctx.length != 28) {
        tx_ctx.active = false;
        return;
    }

    char address[21] = {0};
    char amount_str[32] = {0};

    memcpy(address, tx_ctx.buffer, 20);

    uint64_t amount = 0;
    for (int i = 20; i < 28; i++) {
        amount = (amount << 8) | tx_ctx.buffer[i];
    }

    if (!validate_amount(amount)) {
        tx_ctx.active = false;
        return;
    }

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

    tx_ctx.active = false;
}
