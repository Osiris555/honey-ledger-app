#include "ui.h"
#include "os.h"
#include "cx.h"
#include "ux.h"
#include "os_io_seproxyhal.h"

#define CLA_HONEY 0xE0
#define INS_GET_VERSION 0x01

static uint8_t G_io_apdu_buffer[IO_APDU_BUFFER_SIZE];

void handle_get_version(void) {
    G_io_apdu_buffer[0] = 0x01; // major
    G_io_apdu_buffer[1] = 0x00; // minor
    G_io_apdu_buffer[2] = 0x00; // patch
    io_send_response_buffers(G_io_apdu_buffer, 3, SW_OK);
}

void handle_apdu(uint8_t cla, uint8_t ins) {
    if (cla != CLA_HONEY) {
        THROW(SW_CLA_NOT_SUPPORTED);
    }

    switch (ins) {
        case INS_GET_VERSION:
            handle_get_version();
            break;
        default:
            THROW(SW_INS_NOT_SUPPORTED);
    }
}

__attribute__((noreturn))
void app_main(void) {
    for (;;) {
        unsigned int rx = io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0);
        if (rx == 0) continue;

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];

        BEGIN_TRY {
            TRY {
                handle_apdu(cla, ins);
            }
            CATCH_OTHER(e) {
                G_io_apdu_buffer[0] = (e >> 8) & 0xFF;
                G_io_apdu_buffer[1] = e & 0xFF;
                io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
            }
            FINALLY {
            }
        }
        END_TRY;
    }
}

__attribute__((noreturn))
void main(void) {
    UX_INIT();
    os_boot();
    app_main();
}
