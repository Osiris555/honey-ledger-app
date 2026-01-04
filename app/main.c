#include "os.h"
#include "cx.h"
#include "ux.h"

#include "honey.h"
#include "tx.h"
#include "ui.h"

#include <string.h>

unsigned char G_io_apdu_buffer[IO_APDU_BUFFER_SIZE];

static cx_ecfp_private_key_t private_key;
static cx_ecfp_public_key_t public_key;

static void generate_keypair(void) {
    cx_ecfp_generate_pair(
        CX_CURVE_256K1,
        &public_key,
        &private_key,
        1
    );
}

static void handle_get_public_key(uint16_t *tx) {
    memcpy(G_io_apdu_buffer, public_key.W, PUBKEY_LEN);
    *tx = PUBKEY_LEN;
}

static void handle_sign_tx(uint16_t rx, uint16_t *tx) {
    if (rx < sizeof(honey_tx_t))
        THROW(SW_WRONG_LENGTH);

    honey_tx_t tx_obj;
    memcpy(&tx_obj, G_io_apdu_buffer + 5, sizeof(honey_tx_t));

    // 🔐 USER CONFIRMATION
    ui_confirm_tx(&tx_obj);

    uint8_t hash[32];
    honey_tx_hash(&tx_obj, hash);

    uint8_t sig[72];
    unsigned int sig_len = sizeof(sig);

    cx_ecdsa_sign(
        &private_key,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        hash,
        32,
        sig,
        &sig_len
    );

    memcpy(G_io_apdu_buffer, sig, sig_len);
    *tx = sig_len;
}

static void handle_apdu(uint8_t ins, uint16_t rx, uint16_t *tx) {
    switch (ins) {

        case INS_GET_PUBLIC_KEY:
            handle_get_public_key(tx);
            break;

        case INS_SIGN_TX:
            handle_sign_tx(rx, tx);
            break;

        default:
            THROW(SW_INS_NOT_SUPPORTED);
    }
}

void app_main(void) {
    uint16_t rx = 0;
    uint16_t tx = 0;

    generate_keypair();
    ui_idle();

    for (;;) {
        rx = io_exchange(CHANNEL_APDU, tx);
        tx = 0;

        if (rx == 0)
            continue;

        uint8_t ins = G_io_apdu_buffer[1];

        BEGIN_TRY {
            TRY {
                handle_apdu(ins, rx, &tx);
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
