#pragma once
#include <stdint.h>

/* ---------- TX STRUCT (TIGHT) ---------- */
typedef struct {
    uint8_t  to[20];
    uint64_t amount;
    uint32_t chain_id;
    uint8_t  nonce[8];
} honey_tx_t;

/* ---------- GLOBAL CONTEXT ---------- */
typedef struct {
    honey_tx_t tx;
    uint8_t    raw_tx[128];
    uint16_t   raw_tx_len;
} honey_ctx_t;

/* ---------- SINGLE GLOBAL INSTANCE ---------- */
extern honey_ctx_t G_honey_ctx;
