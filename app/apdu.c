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

    tx_approved = false;
    ux_confirm_transaction();

    while (!tx_approved) {
        os_sched_yield();
    }

    // ---- DOMAIN SEPARATED HASH ----
    cx_hash_sha256_t sha;
    cx_sha256_init(&sha);

    // Domain string
    cx_hash((cx_hash_t *)&sha, 0,
            (uint8_t *)HONEY_DOMAIN,
            HONEY_DOMAIN_LEN,
            NULL, 0);

    // Version
    uint8_t version = HONEY_VERSION;
    cx_hash((cx_hash_t *)&sha, 0, &version, 1, NULL, 0);

    // Chain ID (big endian)
    uint32_t chain_id = HONEY_CHAIN_ID;
    uint8_t chain_id_be[4] = {
        (chain_id >> 24) & 0xFF,
        (chain_id >> 16) & 0xFF,
        (chain_id >> 8) & 0xFF,
        chain_id & 0xFF
    };
    cx_hash((cx_hash_t *)&sha, 0, chain_id_be, 4, NULL, 0);

    // Transaction type
    uint8_t tx_type = HONEY_TX_TRANSFER;
    cx_hash((cx_hash_t *)&sha, 0, &tx_type, 1, NULL, 0);

    // Recipient
    cx_hash((cx_hash_t *)&sha, 0,
            parsed_tx.recipient,
            20,
            NULL, 0);

    // Amount (final)
    cx_hash((cx_hash_t *)&sha,
            CX_LAST,
            parsed_tx.amount,
            32,
            tx_hash,
            32);

    // ---- Derive private key ----
    uint8_t raw_private_key[32];
    os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        (uint32_t[]){
            44 | 0x80000000,
            60 | 0x80000000,
            0x80000000,
            0,
            0
        },
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

    // ---- Sign ----
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
