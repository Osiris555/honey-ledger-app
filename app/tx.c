#include "tx.h"
#include "cx.h"

#include <string.h>

static void serialize_tx(const honey_tx_t *tx, uint8_t *buf) {
    uint8_t offset = 0;

    buf[offset++] = tx->version;

    memcpy(buf + offset, &tx->nonce, 8);
    offset += 8;

    memcpy(buf + offset, tx->to, 20);
    offset += 20;

    memcpy(buf + offset, tx->amount, 32);
    offset += 32;

    memcpy(buf + offset, &tx->fee, 8);
    offset += 8;

    memcpy(buf + offset, &tx->chain_id, 4);
}

void honey_tx_hash(const honey_tx_t *tx, uint8_t out[32]) {
    uint8_t serialized[HONEY_TX_SIZE];
    uint8_t tmp[32];

    serialize_tx(tx, serialized);

    cx_hash_sha256(serialized, HONEY_TX_SIZE, tmp, 32);
    cx_hash_sha256(tmp, 32, out, 32);
}
