#include "os.h"
#include "cx.h"
#include "honey.h"
#include "ui.h"
#include <string.h>

/*
 APDU payload format (simple, explicit):

 [recipient_len][recipient_bytes...]
 [amount_len][amount_ascii...]
*/

static void handle_sign_tx(uint8_t *io_buffer, uint16_t *tx) {
    uint8_t offset = 5;

    /* Recipient */
    uint8_t rlen = io_buffer[offset++];
    memcpy(G_tx_recipient, &io_buffer[offset], rlen);
    G_tx_recipient[rlen] = 0;
    offset += rlen;

    /* Amount */
    uint8_t alen = io_buffer[offset++];
    memcpy(G_tx_amount, &io_buffer[offset], alen);
    G_tx_amount[alen] = 0;

    /* Show transaction */
    ui_confirm_transaction();

    while (!G_tx_approved) {
        ux_step();
    }

    /* Placeholder signature (next step) */
    io_buffer[0] = 0x90;
    io_buffer[1] = 0x00;
    *tx = 2;
}

/* ================================
 * Dispatcher
 * ================================ */

void handle_apdu(uint8_t *io_buffer, uint16_t *tx) {
    switch (io_buffer[1]) {
        case INS_SIGN_TX:
            handle_sign_tx(io_buffer, tx);
            break;
        default:
            THROW(0x6D00);
    }
}
