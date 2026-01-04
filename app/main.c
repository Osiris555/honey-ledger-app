#include "os.h"
#include "cx.h"
#include "honey.h"

unsigned char G_io_seproxyhal_spi_buffer[IO_SEPROXYHAL_BUFFER_SIZE_B];

static void handle_apdu(uint8_t *apdu) {
    uint8_t cla = apdu[0];
    uint8_t ins = apdu[1];
    uint8_t p1  = apdu[2];
    uint16_t lc = apdu[4];
    uint8_t *data = &apdu[5];

    if (cla != CLA_HONEY) {
        THROW(0x6E00);
    }

    switch (ins) {
        case INS_SIGN_TX:
            handle_sign_tx(p1, data, lc);

            if (p1 == P1_FINISH) {
                memcpy(G_io_seproxyhal_spi_buffer,
                       signature,
                       signature_len);
                io_seproxyhal_send(
                    G_io_seproxyhal_spi_buffer,
                    signature_len,
                    0x9000
                );
            }
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
                io_seproxyhal_init();
                io_seproxyhal_spi_recv(
                    G_io_seproxyhal_spi_buffer,
                    sizeof(G_io_seproxyhal_spi_buffer),
                    0
                );
                handle_apdu(G_io_seproxyhal_spi_buffer);
            }
            CATCH_OTHER(e) {
                io_seproxyhal_send_status(e);
            }
            FINALLY {}
        }
        END_TRY;
    }
}
