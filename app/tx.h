#pragma once

#include <stdint.h>
#include <stddef.h>

/* ---------- DOMAIN SEPARATION ---------- */

/*
 * This domain tag is injected into every signed message.
 * Changing this INVALIDATES all old signatures.
 */
#define HONEY_DOMAIN_TAG "HONEY::TX::V1"

/* ---------- TRANSACTION STRUCT ---------- */

typedef struct {
    uint64_t amount;
    uint64_t fee;
    uint8_t  to[20];
    uint32_t nonce;
} honey_tx_t;

/* ---------- SERIALIZATION ---------- */

/* Serialize tx with domain separation */
size_t honey_tx_serialize(
    const honey_tx_t *tx,
    uint8_t *out,
    size_t out_len
);
