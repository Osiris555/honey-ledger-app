#include "apdu.h"
#include <string.h>

/*
 * GET_VERSION
 * No data allowed.
 */
static bool handle_get_version(
    uint16_t data_len,
    uint16_t *sw
) {
    if (data_len != 0) {
        *sw = SW_WRONG_LENGTH;
        return true;
    }

    /* Version 1.0.0 */
    G_io_apdu_buffer[0] = 1;
    G_io_apdu_buffer[1] = 0;
    G_io_apdu_buffer[2] = 0;

    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 3);
    *sw = SW_OK;
    return true;
}

/*
 * SIGN_TX
 * Minimal skeleton — validates length only.
 */
static bool handle_sign_tx(
    const uint8_t *data,
    uint16_t data_len,
    uint16_t *sw
) {
    /* Minimal expected size (example placeholder) */
    if (data_len < 8) {
        *sw = SW_WRONG_LENGTH;
        return true;
    }

    /* TODO: real parsing & UI confirmation */

    *sw = SW_CONDITIONS_NOT_SATISFIED;
    return true;
}

bool apdu_dispatch(
    uint8_t cla,
    uint8_t ins,
    uint8_t p1,
    uint8_t p2,
    const uint8_t *data,
    uint16_t data_len,
    uint16_t *sw
) {
    (void)p1;
    (void)p2;

    if (cla != CLA_HONEY) {
        *sw = SW_CLA_NOT_SUPPORTED;
        return false;
    }

    switch (ins) {
        case INS_GET_VERSION:
            return handle_get_version(data_len, sw);

        case INS_SIGN_TX:
            return handle_sign_tx(data, data_len, sw);

        default:
            *sw = SW_INS_NOT_SUPPORTED;
            return false;
    }
}
