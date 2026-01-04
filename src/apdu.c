#include "apdu.h"
#include "globals.h"

#include "handler_get_public_key.h"
#include "handler_sign_tx.h"

#include <string.h>

void handle_apdu(uint8_t *apdu_buffer, uint16_t apdu_length) {
    uint8_t cla = apdu_buffer[0];
    uint8_t ins = apdu_buffer[1];
    uint8_t p1  = apdu_buffer[2];
    uint8_t p2  = apdu_buffer[3];
    uint8_t lc  = apdu_buffer[4];

    uint8_t *data = &apdu_buffer[5];

    if (cla != CLA_HONEY) {
        THROW(SW_INS_NOT_SUPPORTED);
    }

    switch (ins) {

        case INS_GET_PUBLIC_KEY:
            handle_get_public_key(p1, p2, data, lc);
            break;

        case INS_SIGN_TX: {
    	    uint64_t amount = read_u64(data_buffer);

    	    ui_sign_tx_start(amount);

    	    while (ux_flow_is_active()) {
        	os_sched_yield();
    }

    	    if (!ui_sign_tx_approved()) {
        	THROW(0x6985); // User rejected
    }

    	    sign_transaction(data_buffer, data_length, response_buffer, &tx);
    	    break;
}

        case INS_SIGN_FINAL:
            handle_sign_final();
            break;

        default:
            THROW(SW_INS_NOT_SUPPORTED);
    }
}
