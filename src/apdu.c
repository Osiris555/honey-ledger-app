#include "apdu.h"
#include "os_io_seproxyhal.h"
#include <string.h>

/* CLA */
#define HONEY_CLA 0xE0

/* INS codes */
#define INS_GET_APP_VERSION 0x01
#define INS_GET_APP_NAME    0x02

/* App metadata */
#define APP_NAME    "Honey Ledger"
#define APP_VERSION "0.2.0"

/* Global APDU buffer */
extern unsigned char G_io_apdu_buffer[IO_APDU_BUFFER_SIZE];

/* Handle APDU */
void handle_apdu(uint8_t *apdu_buffer, uint16_t apdu_length) {
    uint8_t cla = apdu_buffer[OFFSET_CLA];
    uint8_t ins = apdu_buffer[OFFSET_INS];

    /* Enforce correct CLA */
    if (cla != HONEY_CLA) {
        G_io_apdu_buffer[0] = 0x6E;
        G_io_apdu_buffer[1] = 0x00;
        io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
        return;
    }

    switch (ins) {

        /* GET_APP_VERSION */
        case INS_GET_APP_VERSION: {
            size_t len = strlen(APP_VERSION);
            memcpy(G_io_apdu_buffer, APP_VERSION, len);
            G_io_apdu_buffer[len]     = 0x90;
            G_io_apdu_buffer[len + 1] = 0x00;
            io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, len + 2);
            break;
        }

        /* GET_APP_NAME */
        case INS_GET_APP_NAME: {
            size_t len = strlen(APP_NAME);
            memcpy(G_io_apdu_buffer, APP_NAME, len);
            G_io_apdu_buffer[len]     = 0x90;
            G_io_apdu_buffer[len + 1] = 0x00;
            io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, len + 2);
            break;
        }

        /* Unknown INS */
        default:
            G_io_apdu_buffer[0] = 0x6D;
            G_io_apdu_buffer[1] = 0x00;
            io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 2);
            break;
    }
}
