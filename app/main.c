#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ux.h"

#include <string.h>

unsigned char G_io_apdu_buffer[IO_APDU_BUFFER_SIZE];

static cx_ecfp_private_key_t private_key;
static cx_ecfp_public_key_t public_key;

/* Generate deterministic key once */
static void generate_keypair(void) {
    cx_ecfp_generate_pair(
        CX_CURVE_256K1,
        &public_key,
        &private_key,
        1
    );
}

/* Derive Honey address = RIPEMD160(SHA256(pubkey)) */
static void derive_address(uint8_t *out, const uint8_t *pubkey) {
    uint8_t sha256[32];
    cx_hash_sha256(pubkey, PUBKEY_LEN, sha256, 32);
    cx_ripemd160(sha256, 32, out, ADDRESS_LEN);
}

static void handle_get_public_key(uint16_t *tx) {
    uint8_t address[ADDRESS_LEN];

    derive_address(address, public_key.W);

    uint8_t offset = 0;

    memcpy(G_io_apdu_buffer + offset, public_key.W, PUBKEY_LEN);
    offset += PUBKEY_LEN;

    memcpy(G_io_apdu_buffer + offset, address, ADDRESS_LEN);
    offset += ADDRESS_LEN;

    *tx = offset;
}

static void handle_apdu(uint8_t ins, uint16_t *tx) {
    switch (ins) {

        case INS_GET_PUBLIC_KEY:
            handle_get_public_key(tx);
            break;

        default:
            THROW(SW_INS_NOT_SUPPORTED);
    }
}

void app_main(void) {
    uint16_t rx = 0;
    uint16_t tx = 0;

    generate_keypair();

    for (;;) {
        rx = io_exchange(CHANNEL_APDU, tx);
        tx = 0;

        if (rx == 0)
            continue;

        uint8_t ins = G_io_apdu_buffer[1];

        BEGIN_TRY {
            TRY {
                handle_apdu(ins, &tx);
                G_io_apdu_buffer[tx++] = 0x90;
                G_io_apdu_buffer[tx++] = 0x00;
            }
            CATCH_OTHER(e) {
                G_io_apdu_buffer[0] = (e >> 8) & 0xFF;
                G_io_apdu_buffer[1] = e & 0xFF;
                tx = 2;
            }
            FINALLY {}
        }
        END_TRY;
    }
}
