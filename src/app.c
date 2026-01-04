#include "app.h"
#include "apdu.h"
#include "globals.h"

#include <os.h>
#include <os_io_seproxyhal.h>

void app_main(void) {
    for (;;) {
        uint16_t rx = io_exchange(CHANNEL_APDU, 0);

        if (rx == 0) {
            continue;
        }

        uint16_t tx = handle_apdu(G_io_apdu_buffer, rx);
        io_exchange(CHANNEL_APDU, tx);
    }
}
