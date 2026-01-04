#include <os.h>
#include <cx.h>
#include <string.h>

#include "apdu.h"

static uint8_t G_io_apdu_buffer[IO_APDU_BUFFER_SIZE];

void app_main(void) {
    for (;;) {
        // Receive APDU
        unsigned int rx = io_exchange(CHANNEL_APDU, 0);

        if (rx < 5) {
            continue;
        }

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];
        uint8_t p1  = G_io_apdu_buffer[2];
        uint8_t p2  = G_io_apdu_buffer[3];
        uint8_t lc  = G_io_apdu_buffer[4];

        const uint8_t *data = &G_io_apdu_buffer[5];
        uint8_t *tx = G_io_apdu_buffer;

        size_t tx_len = 0;
        uint16_t sw = 0x9000;

        handle_apdu(
            cla,
            ins,
            p1,
            p2,
            lc,
            data,
            tx,
            &tx_len,
            &sw
        );

        // Append status word
        tx[tx_len++] = (sw >> 8) & 0xFF;
        tx[tx_len++] = sw & 0xFF;

        io_exchange(CHANNEL_APDU, tx_len);
    }
}

__attribute__((noreturn))
void main(void) {
    os_boot();

    for (;;) {
        BEGIN_TRY {
            TRY {
                app_main();
            }
            CATCH_OTHER(e) {
                // ignore
            }
            FINALLY {
            }
        }
        END_TRY;
    }
}
