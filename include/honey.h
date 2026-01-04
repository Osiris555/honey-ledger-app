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

#define HONEY_CLA 0xE0
#define INS_SIGN_TX 0x01

#define MAX_TX_BUFFER 128

typedef struct {
    uint64_t amount;
    uint32_t chain_id;
    uint8_t  to[20];
    uint8_t  nonce[8];
} honey_tx_t;

// Global transaction context
typedef struct {
    honey_tx_t tx;
    uint8_t raw[MAX_TX_BUFFER];
    uint32_t raw_len;
    bool ready;
} honey_ctx_t;

extern honey_ctx_t G_honey_ctx;

// UI helpers
void ui_display_tx(void);
void ui_approve_tx(void);
void ui_reject_tx(void);
