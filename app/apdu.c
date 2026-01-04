#include "os.h"
#include "cx.h"
#include "honey.h"
#include <string.h>

uint8_t tx_buffer[MAX_TX_SIZE];
uint16_t tx_buffer_len = 0;
bool tx_in_progress = false;

uint8_t tx_hash[32];
uint8_t signature[72];
uint16_t signature_len;

bool tx_approved = false;
honey_tx_t parsed_tx;

static cx_ecfp_private_key_t private_key;

static void reset_tx(void) {
    tx_buffer_len = 0;
    tx_in_progress = false;
    tx_approved = false;
}

void handle_sign_tx(uint8_t p1, uint8_t *data, uint16_t len) {

    // ---- START ----
    if (p1 == P1_START) {
        reset_tx();
        tx_in_progress = true;
    }

    if (!tx_in_progress) {
        THROW(0x6985); // no transaction started
    }

    // ---- Bounds check ----
    if (tx_buffer_len + len > MAX_TX_SIZE) {
        reset_tx();
        THROW(0x6A84); // not enough memory
    }

    memcpy(tx_buffer + tx_buffer_len, data, len);
    tx_buffer_len += len;

    // ---- CONTINUE ----
    if (p1 == P1_CONTINUE) {
        return;
    }

    // ---- FINISH ----
    if (p1 != P1_FINISH) {
        THROW(0x6A86);
    }

    if (tx_buffer_len < (20 + 32)) {
        reset_tx();
        THROW(0x6700);
    }

    memcpy(parsed_tx.recipient, tx_buffer, 20);
    memcpy(parsed_tx.amount, tx_buffer + 20, 32);

    ux_confirm_transaction();
    while (!tx_approved) {
        os_sched_yield();
    }

    // ---- DOMAIN SEPARATED HASH ----
    cx_hash_sha256_t sha;
    cx_sha256_init(&sha);

    cx_hash((cx_hash_t *)&sha, 0,
            (uint8_t *)HONEY_DOMAIN,
            HONEY_DOMAIN_LEN,
            NULL, 0);

    uint8_t version = HONEY_VERSION;
    cx_hash((cx_hash_t *)&sha, 0, &version, 1, NULL, 0);

    uint32_t chain_id = HONEY_CHAIN_ID;
    uint8_t chain_id_be[4] = {
        chain_id >> 24,
        chain_id >> 16,
        chain_id >> 8,
        chain_id
    };
    cx_hash((cx_hash_t *)&sha, 0, chain_id_be, 4, NULL, 0);

    uint8_t tx_type = 0x01;
    cx_hash((cx_hash_t *)&sha, 0, &tx_type, 1, NULL, 0);

    cx_hash((cx_hash_t *)&sha, 0,
            parsed_tx.recipient,
            20,
            NULL, 0);

    cx_hash((cx_hash_t *)&sha,
            CX_LAST,
            parsed_tx.amount,
            32,
            tx_hash,
            32);

    // ---- Key derivation ----
    uint8_t raw_key[32];
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
        raw_key,
        NULL
    );

    cx_ecfp_init_private_key(
        CX_CURVE_256K1,
        raw_key,
        32,
        &private_key
    );

    signature_len = cx_ecdsa_sign(
        &private_key,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        tx_hash,
        32,
        signature,
        sizeof(signature)
    );

    explicit_bzero(raw_key, sizeof(raw_key));
    reset_tx();
}
