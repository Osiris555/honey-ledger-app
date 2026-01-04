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
#include "cx.h"

// ---- Protocol constants ----
#define HONEY_DECIMALS 18
#define HONEY_VERSION  0x01
#define HONEY_CHAIN_ID 1        // change later (1 = mainnet placeholder)
#define HONEY_TX_TRANSFER 0x01

#define HONEY_DOMAIN "HONEY"
#define HONEY_DOMAIN_LEN 5

// ---- APDU ----
#define CLA_HONEY 0xE0
#define INS_SIGN_TX 0x01

// ---- Transaction ----
typedef struct {
    uint8_t recipient[20];
    uint8_t amount[32];
} honey_tx_t;

extern honey_tx_t parsed_tx;
extern bool tx_approved;

// ---- Signing output ----
extern uint8_t tx_hash[32];
extern uint8_t signature[72];
extern uint16_t signature_len;

// ---- UX ----
void ux_confirm_transaction(void);

// ---- Crypto ----
void sign_transaction(void);

#endif
