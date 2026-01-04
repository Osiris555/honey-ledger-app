#include "os.h"
#include "cx.h"
#include "honey.h"
#include <string.h>

uint8_t tx_hash[32];
uint8_t signature[72];
uint16_t signature_len;

static cx_ecfp_private_key_t private_key;

void handle_sign_tx(uint8_t *data, uint16_t len) {
    if (len < (20 + 32)) {
        THROW(0x6700);
    }

    memcpy(parsed_tx.recipient, data, 20);
    memcpy(parsed_tx.amount, data + 20, 32);

    // UX confirmation
    ux_confirm_transaction();

    while (!tx_approved) {
        os_sched_yield();
    }

    // Hash transaction
    cx_hash_sha256_t sha;
    cx_sha256_init(&sha);

    cx_hash(
        (cx_hash_t *)&sha,
        0,
        parsed_tx.recipient,
        20,
        NULL,
        0
    );

    cx_hash(
        (cx_hash_t *)&sha,
        CX_LAST,
        parsed_tx.amount,
        32,
        tx_hash,
        32
    );

    // Load private key from Ledger seed
    uint8_t raw_private_key[32];
    os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        (uint32_t[]){44 | 0x80000000, 60 | 0x80000000, 0x80000000, 0, 0},
        5,
        raw_private_key,
        NULL
    );

    cx_ecfp_init_private_key(
        CX_CURVE_256K1,
        raw_private_key,
        32,
        &private_key
    );

    // Sign
    signature_len = cx_ecdsa_sign(
        &private_key,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        tx_hash,
        32,
        signature,
        sizeof(signature)
    );

    explicit_bzero(raw_private_key, sizeof(raw_private_key));
}
