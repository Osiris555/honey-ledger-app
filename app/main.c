#include <string.h>
#include "apdu.h"

unsigned char G_io_apdu_buffer[260];

int main(void) {
    uint16_t rx = 0;
    uint16_t sw = SW_OK;

    for (;;) {
        rx = io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0);
        if (rx < 5) {
            io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0);
            continue;
        }

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];
        uint8_t p1  = G_io_apdu_buffer[2];
        uint8_t p2  = G_io_apdu_buffer[3];
        uint8_t lc  = G_io_apdu_buffer[4];

        if (rx != (uint16_t)(5 + lc)) {
            sw = SW_WRONG_LENGTH;
            goto send_sw;
        }

        if (!apdu_dispatch(
            cla,
            ins,
            p1,
            p2,
            &G_io_apdu_buffer[5],
            lc,
            &sw
        )) {
            /* sw already set */
        }

send_sw:
        G_io_apdu_buffer[0] = (uint8_t)(sw >> 8);
        G_io_apdu_buffer[1] = (uint8_t)(sw & 0xFF);
        io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
    }
}
