#include "os.h"
#include "cx.h"

#include "handler_sign_tx.h"

#include <string.h>

#define CLA_HONEY 0xE0
#define INS_SIGN_TX 0x02

void handle_apdu(uint8_t *apdu, uint16_t len) {
    uint8_t cla = apdu[0];
    uint8_t ins = apdu[1];
    uint8_t p1  = apdu[2];
    uint8_t p2  = apdu[3];
    uint8_t lc  = apdu[4];

    uint8_t *data = apdu + 5;

    if (cla != CLA_HONEY) {
        return;
    }

    switch (ins) {
        case INS_SIGN_TX:
            handle_sign_tx_chunk(p1, data, lc);
            break;
        default:
            break;
    }
}
