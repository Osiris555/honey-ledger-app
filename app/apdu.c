#include "apdu.h"

#include "os.h"
#include "cx.h"

#include <string.h>

// Global APDU buffer provided by Ledger SDK
extern unsigned char G_io_apdu_buffer[IO_APDU_BUFFER_SIZE];
extern unsigned short G_io_apdu_length;

static void send_version(void) {
    uint8_t version[] = { 0x00, 0x01, 0x00 }; // v0.1.0

    memcpy(G_io_apdu_buffer, version, sizeof(version));
    G_io_apdu_length = sizeof(version);

    // Append status word
    G_io_apdu_buffer[G_io_apdu_length++] = (SW_OK >> 8) & 0xFF;
    G_io_apdu_buffer[G_io_apdu_length++] = SW_OK & 0xFF;
}

void handle_apdu(uint8_t cla, uint8_t ins) {
    if (cla != CLA_HONEY) {
        G_io_apdu_buffer[0] = (SW_CLA_NOT_SUPPORTED >> 8) & 0xFF;
        G_io_apdu_buffer[1] = SW_CLA_NOT_SUPPORTED & 0xFF;
        G_io_apdu_length = 2;
        return;
    }

    switch (ins) {
        case INS_GET_VERSION:
            send_version();
            break;

        default:
            G_io_apdu_buffer[0] = (SW_INS_NOT_SUPPORTED >> 8) & 0xFF;
            G_io_apdu_buffer[1] = SW_INS_NOT_SUPPORTED & 0xFF;
            G_io_apdu_length = 2;
            break;
    }
}
