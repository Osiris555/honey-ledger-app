#pragma once

#include <stdint.h>

#define HONEY_TX_MAX_MEMO_LEN   64
#define HONEY_TX_VERSION       1

typedef struct {
    uint8_t  version;
    uint8_t  chain_id;
    uint64_t nonce;
    uint64_t amount;
    uint64_t fee;
    uint8_t  to[20];
    uint8_t  memo_len;
    uint8_t  memo[HONEY_TX_MAX_MEMO_LEN];
} honey_tx_t;

int honey_tx_validate(const honey_tx_t *tx);
void honey_tx_hash(const honey_tx_t *tx, uint8_t out[32]);
