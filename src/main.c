#include "os.h"
#include "cx.h"
#include "globals.h"
#include "ui.h"
#include "handler_sign_tx.h"
#include <string.h>

#define CLA 0xE0
#define INS_GET_PUBLIC_KEY 0x02
#define INS_SIGN_TX       0x04

void app_main(void) {
    ui_idle();

    for (;;) {
        unsigned int rx = io_exchange(CHANNEL_APDU, 0);

        if (rx == 0) continue;

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];
        uint8_t p1  = G_io_apdu_buffer[2];
        uint8_t p2  = G_io_apdu_buffer[3];
        uint8_t lc  = G_io_apdu_buffer[4];

        uint8_t *data = G_io_apdu_buffer + 5;

        if (cla != CLA) {
            THROW(0x6E00);
        }

        switch (ins) {

            case INS_GET_PUBLIC_KEY:
                // existing handler assumed
                THROW(0x9000);
                break;

            case INS_SIGN_TX:
                handle_sign_tx(p1, p2, data, lc);
                break;

            default:
                THROW(0x6D00);
        }
    }
}
