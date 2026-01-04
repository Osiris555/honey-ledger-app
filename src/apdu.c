#include "apdu.h"
#include "constants.h"

void handle_apdu(
    uint8_t cla,
    uint8_t ins,
    uint8_t p1,
    uint8_t p2,
    uint8_t lc,
    const uint8_t *data,
    uint8_t *tx,
    size_t *tx_len,
    uint16_t *sw
) {
    (void)p1;
    (void)p2;
    (void)data;

    *tx_len = 0;

    if (cla != CLA_HONEY) {
        *sw = SW_CLA_NOT_SUPPORTED;
        return;
    }

    switch (ins) {

    case INS_GET_VERSION:
        // version: MAJOR.MINOR.PATCH
        tx[0] = 0x01; // major
        tx[1] = 0x00; // minor
        tx[2] = 0x00; // patch
        *tx_len = 3;
        *sw = SW_OK;
        break;

    case INS_PING:
        if (lc != 0) {
            *sw = SW_WRONG_LENGTH;
            return;
        }
        tx[0] = 0xAA;
        tx[1] = 0x55;
        *tx_len = 2;
        *sw = SW_OK;
        break;

    default:
        *sw = SW_INS_NOT_SUPPORTED;
        break;
    }
}
