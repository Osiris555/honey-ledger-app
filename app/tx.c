#include "tx.h"
#include <string.h>

/*
 * Serialized format:
 *
 * [ DOMAIN TAG ]
 * [ amount (8) ]
 * [ fee    (8) ]
 * [ nonce  (4) ]
 * [ to     (20) ]
 *
 * This ensures signatures are bound to:
 * - Honey
 * - Transaction intent
 * - Protocol version
 */

size_t honey_tx_serialize(
    const honey_tx_t *tx,
    uint8_t *out,
    size_t out_len
) {
    size_t offset = 0;
    size_t domain_len = strlen(HONEY_DOMAIN_TAG);

    if (out_len < domain_len + 8 + 8 + 4 + 20) {
        return 0;
    }

    /* Domain separation */
    memcpy(out + offset, HONEY_DOMAIN_TAG, domain_len);
    offset += domain_len;

    /* Amount */
    memcpy(out + offset, &tx->amount, sizeof(tx->amount));
    offset += sizeof(tx->amount);

    /* Fee */
    memcpy(out + offset, &tx->fee, sizeof(tx->fee));
    offset += sizeof(tx->fee);

    /* Nonce */
    memcpy(out + offset, &tx->nonce, sizeof(tx->nonce));
    offset += sizeof(tx->nonce);

    /* Destination */
    memcpy(out + offset, tx->to, sizeof(tx->to));
    offset += sizeof(tx->to);

    return offset;
}
