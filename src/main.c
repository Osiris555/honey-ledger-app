#include "os.h"
#include "cx.h"
#include "ux.h"

#include "handler_sign_tx.h"

unsigned char G_io_seproxyhal_spi_buffer[IO_SEPROXYHAL_BUFFER_SIZE];

#define INS_SIGN_TX 0x02

void app_main(void);

void app_main(void) {
    for (;;) {
        uint16_t rx = io_exchange(CHANNEL_APDU | CHANNEL_SPI, 0);

        if (rx < 5) {
            continue;
        }

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];
        uint16_t lc = G_io_apdu_buffer[4];

        uint8_t *data = &G_io_apdu_buffer[5];

        switch (ins) {
            case INS_SIGN_TX:
                handle_sign_tx(data, lc);
                break;
            default:
                break;
        }
    }
}

__attribute__((section(".boot"))) int main(void) {
    os_boot();
    app_main();
    return 0;
}
