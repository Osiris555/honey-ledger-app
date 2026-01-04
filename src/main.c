#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ux.h"

honey_ctx_t G_honey_ctx;

static void handle_sign_tx(uint8_t *data, uint16_t len) {
    if (len != sizeof(honey_tx_t)) {
        THROW(0x6700); // wrong length
    }

    os_memmove(&G_honey_ctx.tx, data, sizeof(honey_tx_t));
    G_honey_ctx.ready = true;

    ui_display_tx();
}

void handle_apdu(uint8_t *buffer, uint16_t size) {
    uint8_t cla = buffer[0];
    uint8_t ins = buffer[1];
    uint8_t lc  = buffer[4];
    uint8_t *data = buffer + 5;

    if (cla != HONEY_CLA) {
        THROW(0x6E00);
    }

    switch (ins) {
        case INS_SIGN_TX:
            handle_sign_tx(data, lc);
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
                uint8_t apdu_buffer[260];
                uint16_t rx = io_exchange(CHANNEL_APDU, sizeof(apdu_buffer));
                handle_apdu(apdu_buffer, rx);
            }
            CATCH_OTHER(e) {
                io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, e);
            }
            FINALLY {
            }
        }
        END_TRY;
    }
}
