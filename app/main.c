#include "os.h"
#include "cx.h"
#include "honey.h"
#include "apdu.h"

unsigned char G_io_apdu_buffer[260];

void app_main(void) {
    for (;;) {
        uint16_t rx = io_exchange(CHANNEL_APDU | IO_ASYNCH_REPLY, 0);

        if (rx < 5) {
            THROW(0x6700);
        }

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];

        uint16_t tx_len = 0;

        if (cla != CLA_HONEY) {
            THROW(0x6E00);
        }

        switch (ins) {
            case INS_GET_VERSION:
                handle_get_version(G_io_apdu_buffer, &tx_len);
                break;

            case INS_PING:
                handle_ping(G_io_apdu_buffer, &tx_len);
                break;

            default:
                THROW(0x6D00);
        }

        io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, tx_len);
    }
}

__attribute__((noreturn))
void main(void) {
    app_main();
    for (;;) {}
}
