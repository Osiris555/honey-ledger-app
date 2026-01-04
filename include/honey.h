/*
 * Honey Monetary Definitions
 *
 * Base unit: CONE
 * Display unit: HONEY (HNY)
 *
 * 1 HNY = 10^18 CONES
 *
 * - All cryptographic signing uses CONES
 * - UI formatting converts CONES → HNY
 * - No floating-point math is used
 *
 * This model is fixed and protocol-level.
 */

#ifndef HONEY_H
#define HONEY_H

#include <stdint.h>
#include <stdbool.h>

#define HONEY_DECIMALS 18

// APDU constants
#define CLA_HONEY 0xE0
#define INS_SIGN_TX 0x01

typedef struct {
    uint8_t recipient[20];
    uint8_t amount[32]; // uint256, big-endian
} honey_tx_t;

extern honey_tx_t parsed_tx;
extern bool tx_approved;

// UX entry point
void ux_confirm_transaction(void);

#endif
