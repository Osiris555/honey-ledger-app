#include "apdu.h"
#include <string.h>

#include "os.h"
#include "cx.h"

// APDU offsets
#define OFFSET_CLA   0
#define OFFSET_INS   1

// Status words
#define SW_OK                0x9000
#define SW_INS_NOT_SUPPORTED 0x6D00
#define SW_CLA_NOT_SUPPORTED 0x6E00
#define SW_EXECUTION_FAILED  0x6400

// Hardened index macro
#define HARDENED 0x80000000

static uint16_t handle_get_public_key(uint8_t *buffer) {
    cx_ecfp_private_key_t privkey;
    cx_ecfp_public_key_t pubkey;

    uint8_t pubkey_raw[65];

    // BIP32 path: m/44'/9999'/0'/0/0
    uint32_t path[] = {
        44  | HARDENED,
        9999 | HARDENED,
        0   | HARDENED,
        0,
        0
    };

    // Derive private key from Ledger seed
    cx_err_t err = os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        path,
        5,
        privkey.d,
        NULL
    );

    if (err != CX_OK) {
        return 0;
    }

    // Initialize private key structure
    cx_ecfp_init_private_key(
        CX_CURVE_256K1,
        privkey.d,
        32,
        &privkey
    );

    // Generate corresponding public key
    err = cx_ecfp_generate_pair(
        CX_CURVE_256K1,
        &pubkey,
        &privkey,
        1
    );

    if (err != CX_OK) {
        return 0;
    }

    // Export uncompressed public key
    err = cx_ecfp_export_public_key(&pubkey, pubkey_raw, sizeof(pubkey_raw));
    if (err != CX_OK) {
        return 0;
    }

    memcpy(buffer, pubkey_raw, sizeof(pubkey_raw));

    // Wipe private key
    explicit_bzero(&privkey, sizeof(privkey));

    return sizeof(pubkey_raw);
}

void handle_apdu(uint8_t *buffer, uint16_t length) {
    uint16_t tx = 0;

    uint8_t cla = buffer[OFFSET_CLA];
    uint8_t ins = buffer[OFFSET_INS];

    if (cla != CLA_HONEY) {
        buffer[tx++] = (SW_CLA_NOT_SUPPORTED >> 8) & 0xFF;
        buffer[tx++] = SW_CLA_NOT_SUPPORTED & 0xFF;
        io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
        return;
    }

    switch (ins) {
        case INS_GET_PUBLIC_KEY:
            tx = handle_get_public_key(buffer);
            if (tx == 0) {
                buffer[0] = (SW_EXECUTION_FAILED >> 8) & 0xFF;
                buffer[1] = SW_EXECUTION_FAILED & 0xFF;
                io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
                return;
            }
            break;

        default:
            buffer[tx++] = (SW_INS_NOT_SUPPORTED >> 8) & 0xFF;
            buffer[tx++] = SW_INS_NOT_SUPPORTED & 0xFF;
            io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
            return;
    }

    buffer[tx++] = (SW_OK >> 8) & 0xFF;
    buffer[tx++] = SW_OK & 0xFF;

    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx);
}
