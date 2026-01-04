#include "handler_sign_tx.h"
#include "globals.h"
#include "apdu.h"

#include <os.h>
#include <cx.h>
#include <string.h>

/* Hash context lives in globals (Nano S safe) */
static cx_sha256_t tx_hash_ctx;
static uint8_t hashing_initialized = 0;

/*
 * INS_SIGN_TX
 * P1 = 0x00 → first chunk
 * P1 = 0x01 → continuation
 */
void handle_sign_tx(uint8_t p1, uint8_t p2, uint8_t *data, uint8_t data_len) {
    UNUSED(p2);

    if (p1 == 0x00) {
        /* Initialize hash */
        cx_sha256_init(&tx_hash_ctx);
        hashing_initialized = 1;
    } else if (p1 != 0x01) {
        THROW(SW_WRONG_P1P2);
    }

    if (!hashing_initialized) {
        THROW(SW_SECURITY_STATUS_NOT_SATISFIED);
    }

    /* Hash incoming chunk */
    cx_hash(
        (cx_hash_t *)&tx_hash_ctx,
        0,
        data,
        data_len,
        NULL,
        0
    );

    /* No response data, just OK */
    G_io_apdu_buffer[0] = 0x90;
    G_io_apdu_buffer[1] = 0x00;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
}

/*
 * INS_SIGN_FINAL
 * Finalizes hash and signs it
 */
void handle_sign_final(void) {
    uint8_t hash[32];
    uint8_t signature[72];
    size_t sig_len = sizeof(signature);

    if (!hashing_initialized) {
        THROW(SW_SECURITY_STATUS_NOT_SATISFIED);
    }

    /* Finalize hash */
    cx_hash(
        (cx_hash_t *)&tx_hash_ctx,
        CX_LAST,
        NULL,
        0,
        hash,
        sizeof(hash)
    );

    hashing_initialized = 0;

    /* Sign using the app’s private key */
    cx_ecfp_private_key_t private_key;
    cx_ecfp_public_key_t public_key;

    os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        (uint32_t[]){44 | 0x80000000, 0x80000000, 0x80000000, 0, 0},
        5,
        private_key.d,
        NULL
    );

    cx_ecfp_init_private_key(
        CX_CURVE_256K1,
        private_key.d,
        32,
        &private_key
    );

    cx_ecdsa_sign(
        &private_key,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        hash,
        sizeof(hash),
        signature,
        &sig_len
    );

    /* Return signature */
    memcpy(G_io_apdu_buffer, signature, sig_len);
    G_io_apdu_buffer[sig_len]     = 0x90;
    G_io_apdu_buffer[sig_len + 1] = 0x00;

    io_exchange(
        CHANNEL_APDU | IO_RETURN_AFTER_TX,
        sig_len + 2
    );
}
