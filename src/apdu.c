#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ui.h"
#include <string.h>

/* ================================
 * Bech32 helpers (minimal)
 * ================================ */

extern void bech32_encode(
    char *output,
    const char *hrp,
    const uint8_t *data,
    size_t data_len
);

/* ================================
 * GET_ADDRESS handler
 * ================================ */

static void handle_get_address(uint8_t *io_buffer, uint16_t *tx) {
    cx_ecfp_public_key_t pubkey;
    uint8_t pubkey_raw[33];

    /* Generate public key (example path m/44'/999'/0'/0/0) */
    bip32_path_t path = {
        .length = 5,
        .path = {44 | 0x80000000,
                 999 | 0x80000000,
                 0 | 0x80000000,
                 0,
                 0}
    };

    cx_ecfp_generate_pair(
        CX_CURVE_SeCP256K1,
        &pubkey,
        NULL,
        path.path,
        path.length
    );

    memcpy(pubkey_raw, pubkey.W + 1, 33);

    /* Convert to Bech32 */
    bech32_encode(
        G_honey_address,
        HONEY_HRP,
        pubkey_raw,
        sizeof(pubkey_raw)
    );

    /* Show address on device */
    ui_display_address();

    /* Wait for user */
    while (!G_address_approved) {
        ux_step();
    }

    /* Return address */
    uint16_t len = strlen(G_honey_address);
    memcpy(io_buffer, G_honey_address, len);
    *tx = len;
}

/* ================================
 * APDU Dispatcher
 * ================================ */

void handle_apdu(uint8_t *io_buffer, uint16_t *tx) {
    uint8_t ins = io_buffer[1];

    switch (ins) {
        case INS_GET_ADDRESS:
            handle_get_address(io_buffer, tx);
            break;

        default:
            THROW(0x6D00); // INS not supported
    }
}
