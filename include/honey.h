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

// ---- Protocol ----
#define HONEY_DECIMALS 18
#define HONEY_VERSION  0x01
#define HONEY_CHAIN_ID 1

#define HONEY_DOMAIN "HONEY"
#define HONEY_DOMAIN_LEN 5

// ---- APDU ----
#define CLA_HONEY 0xE0
#define INS_SIGN_TX 0x01

#define P1_START    0x00
#define P1_CONTINUE 0x01
#define P1_FINISH   0x02

// ---- Limits ----
#define MAX_TX_SIZE 512

// ---- Transaction ----
typedef struct {
    uint8_t recipient[20];
    uint8_t amount[32];
} honey_tx_t;

extern honey_tx_t parsed_tx;

// ---- Buffering ----
extern uint8_t tx_buffer[MAX_TX_SIZE];
extern uint16_t tx_buffer_len;
extern bool tx_in_progress;

// ---- Signing ----
extern uint8_t tx_hash[32];
extern uint8_t signature[72];
extern uint16_t signature_len;
extern bool tx_approved;

// ---- UX ----
void ux_confirm_transaction(void);

// ---- APDU ----
void handle_sign_tx(uint8_t p1, uint8_t *data, uint16_t len);

#endif
