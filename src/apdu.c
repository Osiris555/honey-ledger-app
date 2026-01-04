#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ui.h"
#include <string.h>

/* ================================
 * Globals
 * ================================ */

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

    uint8_t private_key_data[32];

    os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        path,
        5,
        private_key_data,
        NULL
    );

    cx_ecfp_init_private_key(
        CX_CURVE_256K1,
        private_key_data,
        32,
        &G_private_key
    );

    explicit_bzero(private_key_data, sizeof(private_key_data));
}

/* ================================
 * Signing
 * ================================ */

static void sign_transaction(uint8_t *io_buffer, uint16_t *tx) {
    uint8_t hash[32];
    cx_sha256_t sha;

    /* Build hash = SHA256(recipient || amount) */
    cx_sha256_init(&sha);
    cx_hash(
        (cx_hash_t *)&sha,
        CX_LAST,
        (uint8_t *)G_tx_recipient,
        strlen(G_tx_recipient),
        hash,
        sizeof(hash)
    );

    cx_hash(
        (cx_hash_t *)&sha,
        CX_LAST,
        (uint8_t *)G_tx_amount,
        strlen(G_tx_amount),
        hash,
        sizeof(hash)
    );

    /* Sign */
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
 * APDU handler
 * ================================ */

static void handle_sign_tx(uint8_t *io_buffer, uint16_t *tx) {
    uint8_t offset = 5;

    /* Recipient */
    uint8_t rlen = io_buffer[offset++];
    memcpy(G_tx_recipient, &io_buffer[offset], rlen);
    G_tx_recipient[rlen] = 0;
    offset += rlen;

    /* Amount */
    uint8_t alen = io_buffer[offset++];
    memcpy(G_tx_amount, &io_buffer[offset], alen);
    G_tx_amount[alen] = 0;

    /* UI */
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
