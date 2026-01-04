#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ux.h"

static honey_tx_t current_tx;

static void handle_sign_tx(uint8_t *data, uint16_t length) {
    if (length == 0 || length > MAX_TX_SIZE) {
        THROW(SW_WRONG_LENGTH);
    }

    os_memmove(current_tx.data, data, length);
    current_tx.length = length;

    ux_confirm_transaction();

    if (!tx_approved) {
        THROW(SW_CONDITIONS_NOT_SATISFIED);
    }

    cx_ecfp_private_key_t priv;
    uint8_t sig[SIG_LEN];

    // NOTE: Placeholder key derivation (replace with BIP32 later)
    cx_ecfp_init_private_key(CX_CURVE_256K1, (uint8_t *)"dummykeydummykeydummykeydummy!", 32, &priv);

    cx_ecdsa_sign(
        &priv,
        CX_RND_RFC6979 | CX_LAST,
        CX_SHA256,
        current_tx.data,
        current_tx.length,
        sig,
        SIG_LEN,
        NULL
    );

    G_io_apdu_buffer[0] = SIG_LEN;
    os_memmove(G_io_apdu_buffer + 1, sig, SIG_LEN);
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, SIG_LEN + 1);
}

void app_main(void) {
    for (;;) {
        unsigned int rx = io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0);
        if (rx == 0) continue;

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];

        if (cla != HONEY_CLA) {
            THROW(SW_INS_NOT_SUPPORTED);
        }

        switch (ins) {
            case INS_SIGN_TX:
                handle_sign_tx(G_io_apdu_buffer + 5, rx - 5);
                break;
            default:
                THROW(SW_INS_NOT_SUPPORTED);
        }
    }
}
