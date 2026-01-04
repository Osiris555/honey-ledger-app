#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CLA_HONEY  0xE0

#define INS_GET_VERSION 0x01
#define INS_SIGN_TX     0x02

#define SW_OK                   0x9000
#define SW_WRONG_LENGTH         0x6700
#define SW_INS_NOT_SUPPORTED    0x6D00
#define SW_CLA_NOT_SUPPORTED    0x6E00
#define SW_CONDITIONS_NOT_SATISFIED 0x6985
#define SW_UNKNOWN              0x6F00

/*
 * Dispatch APDU command.
 * Returns true if command was handled.
 */
bool apdu_dispatch(
    uint8_t cla,
    uint8_t ins,
    uint8_t p1,
    uint8_t p2,
    const uint8_t *data,
    uint16_t data_len,
    uint16_t *sw
);
