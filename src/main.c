#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ux.h"

honey_ctx_t G_honey_ctx;
honey_settings_t G_honey_settings = {
    .blind_signing_enabled = false
};

static void handle_sign_tx(uint8_t *data, uint16_t len) {
    if (!G_honey_settings.blind_signing_enabled &&
        len != sizeof(honey_tx_t)) {
        THROW(0x6985); // conditions not satisfied
    }

    if (len == sizeof(honey_tx_t)) {
        os_memmove(&G_honey_ctx.tx, data, sizeof(honey_tx_t));
        G_honey_ctx.ready = true;
        ui_display_tx();
    } else {
        // Blind signing path
        ui_show_blind_status();
    }
}

static void handle_get_settings(void) {
    uint8_t out[1] = { G_honey_settings.blind_signing_enabled };
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 1);
}

static void handle_set_blind(uint8_t *data, uint16_t len) {
    if (len != 1) THROW(0x6700);
    G_honey_settings.blind_signing_enabled = data[0] ? true : false;
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x9000);
}

void handle_apdu(uint8_t *buffer, uint16_t size) {
    uint8_t cla = buffer[0];
    uint8_t ins = buffer[1];
    uint8_t lc  = buffer[4];
    uint8_t *data = buffer + 5;

    if (cla != HONEY_CLA) THROW(0x6E00);

    switch (ins) {
        case INS_SIGN_TX:
            handle_sign_tx(data, lc);
            break;
        case INS_GET_SETTINGS:
            handle_get_settings();
            break;
        case INS_SET_BLIND:
            handle_set_blind(data, lc);
            break;
        default:
            THROW(0x6D00);
    }
}

int main(void) {
    os_boot();

    for (;;) {
        BEGIN_TRY {
            TRY {
                uint8_t apdu[260];
                uint16_t rx = io_exchange(CHANNEL_APDU, sizeof(apdu));
                handle_apdu(apdu, rx);
            }
            CATCH_OTHER(e) {
                io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, e);
            }
            FINALLY {}
        }
        END_TRY;
    }
}
