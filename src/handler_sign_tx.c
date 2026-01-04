#include "handler_sign_tx.h"
#include "os.h"
#include "cx.h"
#include "globals.h"
#include "ui.h"
#include <string.h>

static uint8_t tx_hash[32];
static uint8_t signature[64];
static cx_ecfp_private_key_t private_key;

void handle_sign_tx(uint8_t p1, uint8_t p2, uint8_t *data, uint16_t len) {

    // Hash transaction bytes (SHA-256)
    cx_hash_sha256(data, len, tx_hash, sizeof(tx_hash));

    // Ask user for approval (blocking UX flow)
    ui_approve_tx();

    // Derive private key from BIP32 path
    os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        bip32_path,
        BIP32_PATH_LEN,
        private_key.d,
        NULL
    );
    private_key.curve = CX_CURVE_256K1;

    // Sign the hash (deterministic)
    cx_ecdsa_sign(
        &private_key,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        tx_hash,
        sizeof(tx_hash),
        signature,
        NULL
    );

    // Send signature back to host
    memcpy(G_io_apdu_buffer, signature, sizeof(signature));
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, sizeof(signature));
}
