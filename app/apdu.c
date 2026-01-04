#include "os.h"
#include "honey.h"
#include <string.h>

#define APDU_MIN_LENGTH (20 + 32)

void handle_sign_tx(uint8_t *data, uint16_t len) {
    if (len < APDU_MIN_LENGTH) {
        THROW(0x6700); // Wrong length
    }

    // Parse recipient (20 bytes)
    memcpy(parsed_tx.recipient, data, 20);

    // Parse amount (32 bytes, uint256 BE)
    memcpy(parsed_tx.amount, data + 20, 32);

    // Launch UX confirmation
    ux_confirm_transaction();

    // Wait for approval
    while (!tx_approved) {
        os_sched_yield();
    }
}
