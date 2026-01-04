#pragma once

#include <stdint.h>

#define HONEY_TX_VERSION 0x01
#define HONEY_TX_SIZE 73

typedef struct {
    uint8_t  version;
    uint64_t nonce;
    uint8_t  to[20];
    uint8_t  amount[32];   // uint256 LE (18 decimals)
    uint64_t fee;
    uint32_t chain_id;
} honey_tx_t;

void honey_tx_hash(const honey_tx_t *tx, uint8_t out[32]);
