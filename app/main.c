#include "os.h"
#include "cx.h"
#include "honey.h"

unsigned char G_io_seproxyhal_spi_buffer[IO_SEPROXYHAL_BUFFER_SIZE_B];

honey_tx_t parsed_tx;

static void handle_apdu(uint8_t *apdu_buffer, uint16_t apdu_length) {
    uint8_t cla = apdu_buffer[0];
    uint8_t ins = apdu_buffer[1];
    uint16_t lc = apdu_buffer[4];
    uint8_t *data = &apdu_buffer[5];

    if (cla != CLA_HONEY) {
        THROW(0x6E00); // CLA not supported
    }

    switch (ins) {
        case INS_SIGN_TX:
            handle_sign_tx(data, lc);
            break;

        default:
            THROW(0x6D00); // INS not supported
    }
}

int main(void) {
    os_boot();

    for (;;) {
        BEGIN_TRY {
            TRY {
                io_seproxyhal_init();
                io_seproxyhal_spi_recv(G_io_seproxyhal_spi_buffer, sizeof(G_io_seproxyhal_spi_buffer), 0);
                handle_apdu(G_io_seproxyhal_spi_buffer, sizeof(G_io_seproxyhal_spi_buffer));
            }
            CATCH_OTHER(e) {
                io_seproxyhal_send_status(e);
            }
            FINALLY {
            }
        }
        END_TRY;
    }
}
