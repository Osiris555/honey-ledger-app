#include "handler_sign_tx.h"
#include "ui.h"

#include "os.h"
#include "cx.h"

#include <string.h>
#include <stdio.h>

#define P1_FIRST 0x00
#define P1_MORE  0x01
#define P1_LAST  0x02

static tx_context_t tx_ctx;

static uint8_t tx_hash[32];
static uint8_t signature[72];
static uint32_t signature_len;

static cx_ecfp_private_key_t private_key;

static void hash_transaction(void) {
    cx_hash_sha256(tx_ctx.buffer, tx_ctx.length, tx_hash, 32);
}

void sign_transaction(void) {
    hash_transaction();

    os_derive_bip32(
        CX_CURVE_256K1,
        (uint32_t[]){44 | 0x80000000, 999 | 0x80000000, 0x80000000, 0, 0},
        5,
        (uint8_t *)&private_key,
        NULL
    );

    cx_ecdsa_sign(
        &private_key,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        tx_hash,
        32,
        signature,
        &signature_len,
        NULL
    );

    explicit_bzero(&private_key, sizeof(private_key));
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

    uint64_t divisor = 1000000000000000000ULL;
    uint64_t whole = amount / divisor;
    uint64_t frac = amount % divisor;

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
