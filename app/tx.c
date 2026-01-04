#include "tx.h"
#include "cx.h"
#include "os.h"

#include <string.h>

/* ---------- VALIDATION ---------- */

int honey_tx_validate(const honey_tx_t *tx) {

    if (tx == NULL)
        return 0;

    if (tx->version != HONEY_TX_VERSION)
        return 0;

    if (tx->memo_len > HONEY_TX_MAX_MEMO_LEN)
        return 0;

    if (tx->amount == 0)
        return 0;

    if (tx->fee == 0)
        return 0;

    // Prevent overflow: amount + fee must not wrap
    if (tx->amount + tx->fee < tx->amount)
        return 0;

    // Prevent sending to zero address
    int zero = 1;
    for (int i = 0; i < 20; i++) {
        if (tx->to[i] != 0) {
            zero = 0;
            break;
        }
    }

    if (zero)
        return 0;

    return 1;
}

/* ---------- HASHING ---------- */

void honey_tx_hash(const honey_tx_t *tx, uint8_t out[32]) {

    cx_sha256_t ctx;
    cx_sha256_init(&ctx);

    cx_hash(
        (cx_hash_t *)&ctx,
        0,
        (uint8_t *)tx,
        sizeof(honey_tx_t),
        NULL,
        0
    );

    cx_hash(
        (cx_hash_t *)&ctx,
        CX_LAST,
        NULL,
        0,
        out,
        32
    );
}
