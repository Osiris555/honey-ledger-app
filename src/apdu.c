#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ui.h"
#include <string.h>

/* ================================
 * Globals
 * ================================ */

char     G_tx_recipient[HONEY_ADDR_LEN];
char     G_tx_amount[32];
uint32_t G_tx_nonce;
bool     G_tx_approved = false;

cx_ecfp_private_key_t G_private_key;

/* ================================
 * Key derivation
 * ================================ */

static void derive_private_key(void) {
    uint32_t path[5] = {
        44 | 0x80000000,
        118 | 0x80000000,
        0  | 0x80000000,
        0,
        0
    };

    uint8_t key_data[32];

    os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        path,
        5,
        key_data,
        NULL
    );

    cx_ecfp_init_private_key(
        CX_CURVE_256K1,
        key_data,
        32,
        &G_private_key
    );

    explicit_bzero(key_data, sizeof(key_data));
}

/* ================================
 * Signing
 * ================================ */

static void sign_transaction(uint8_t *io_buffer, uint16_t *tx) {
    uint8_t hash[32];
    cx_sha256_t sha;

    cx_sha256_init(&sha);

    /* chain-id */
    cx_hash(
        (cx_hash_t *)&sha,
        0,
        (uint8_t *)HONEY_CHAIN_ID,
        strlen(HONEY_CHAIN_ID),
        NULL,
        0
    );

    /* recipient */
    cx_hash(
        (cx_hash_t *)&sha,
        0,
        (uint8_t *)G_tx_recipient,
        strlen(G_tx_recipient),
        NULL,
        0
    );

    /* amount */
    cx_hash(
        (cx_hash_t *)&sha,
        0,
        (uint8_t *)G_tx_amount,
        strlen(G_tx_amount),
        NULL,
        0
    );

    /* nonce */
    cx_hash(
        (cx_hash_t *)&sha,
        CX_LAST,
        (uint8_t *)&G_tx_nonce,
        sizeof(G_tx_nonce),
        hash,
        sizeof(hash)
    );

    derive_private_key();

    uint32_t sig_len = 0;
    cx_ecdsa_sign(
        &G_private_key,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        hash,
        sizeof(hash),
        io_buffer,
        &sig_len,
        NULL
    );

    explicit_bzero(&G_private_key, sizeof(G_private_key));

    io_buffer[sig_len++] = 0x90;
    io_buffer[sig_len++] = 0x00;
    *tx = sig_len;
}

/* ================================
 * APDU
 * ================================ */

static void handle_sign_tx(uint8_t *io_buffer, uint16_t *tx) {
    uint8_t offset = 5;

    /* recipient */
    uint8_t rlen = io_buffer[offset++];
    memcpy(G_tx_recipient, &io_buffer[offset], rlen);
    G_tx_recipient[rlen] = 0;
    offset += rlen;

    /* amount */
    uint8_t alen = io_buffer[offset++];
    memcpy(G_tx_amount, &io_buffer[offset], alen);
    G_tx_amount[alen] = 0;
    offset += alen;

    /* nonce */
    memcpy(&G_tx_nonce, &io_buffer[offset], sizeof(uint32_t));

    /* UI */
    G_tx_approved = false;
    ui_confirm_transaction();
    while (!G_tx_approved) {
        ux_step();
    }

    sign_transaction(io_buffer, tx);
}

/* ================================
 * Dispatcher
 * ================================ */

void handle_apdu(uint8_t *io_buffer, uint16_t *tx) {
    switch (io_buffer[1]) {
        case INS_SIGN_TX:
            handle_sign_tx(io_buffer, tx);
            break;
        default:
            THROW(0x6D00);
    }
}
