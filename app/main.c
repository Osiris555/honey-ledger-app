#include "os.h"
#include "cx.h"

#include "apdu.h"

unsigned char G_io_apdu_buffer[IO_APDU_BUFFER_SIZE];
unsigned short G_io_apdu_length;

static void app_main(void) {
    for (;;) {
        // Receive APDU
        unsigned short rx = io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0);

        if (rx < 5) {
            continue; // Invalid APDU
        }

        uint8_t cla = G_io_apdu_buffer[0];
        uint8_t ins = G_io_apdu_buffer[1];

        handle_apdu(cla, ins);

        // Send response
        io_exchange(CHANNEL_APDU, G_io_apdu_length);
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
                // Reset on error
                os_sched_exit(-1);
            }
            FINALLY {
            }
        }
        END_TRY;
    }
}
