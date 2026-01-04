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
/*
 * Transaction amounts are provided in CONES (uint64 / big-int).
 *
 * The Ledger device signs the raw CONE value exactly as received.
 * Any UI formatting to HNY occurs AFTER parsing and does not
 * affect the signed payload.
 *
 * This guarantees full 18-decimal precision on-chain.
 */

        case INS_SIGN_TX: {
    	    const uint8_t *ptr = data_buffer;

    	    char recipient[64];
    	    memcpy(recipient, ptr, 32);
    	    recipient[32] = '\0';
    	    ptr += 32;

    	    uint64_t amount = read_u64(ptr);
    	    ptr += 8;

    	    uint64_t fee = read_u64(ptr);

    	    ui_sign_tx_start(recipient, amount, fee);

    	    while (ux_flow_is_active()) {
        	os_sched_yield();
    }

    	    if (!ui_sign_tx_approved()) {
        	THROW(0x6985); // User rejected
    }

    	    sign_transaction(
        	data_buffer,
        	data_length,
        	response_buffer,
        	&tx
    	    );
    	    break;
}


        case INS_SIGN_FINAL:
            handle_sign_final();
            break;

        default:
            THROW(SW_INS_NOT_SUPPORTED);
    }
}
