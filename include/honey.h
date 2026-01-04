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

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define HONEY_CLA  0xE0

// Instructions
#define INS_GET_VERSION   0x01
#define INS_GET_PUBKEY    0x02
#define INS_SIGN_TX       0x04

// Status words
#define SW_OK             0x9000
#define SW_INS_NOT_SUPPORTED 0x6D00
#define SW_WRONG_LENGTH   0x6700
#define SW_CONDITIONS_NOT_SATISFIED 0x6985
#define SW_WRONG_DATA     0x6A80

// Limits
#define MAX_TX_SIZE 256
#define PUBKEY_LEN  32
#define SIG_LEN     64

// Honey uses 18 decimals
#define HONEY_DECIMALS 18

typedef struct {
    uint8_t recipient[20];   // address
    uint8_t amount[32];      // uint256
} honey_tx_t;

// Approval flag
extern bool tx_approved;

// Parsed tx exposed to UX
extern honey_tx_t parsed_tx;
