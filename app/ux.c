#include "os.h"
#include "ux.h"
#include "honey.h"
#include "nbgl_use_case.h"
#include <string.h>

static char amount_str[32];
static char address_str[48];

static void format_address(uint8_t *addr) {
    snprintf(address_str, sizeof(address_str),
        "%02X%02X…%02X%02X",
        addr[0], addr[1], addr[18], addr[19]);
}

static void format_amount(uint64_t amount) {
    // Display as HONEY with 18 decimals simplified
    snprintf(amount_str, sizeof(amount_str),
        "%llu CONES", (unsigned long long)amount);
}

void ui_display_tx(void) {
    format_amount(G_honey_ctx.tx.amount);
    format_address(G_honey_ctx.tx.to);

    nbgl_useCaseReview(
        "Sign Honey Tx",
        amount_str,
        address_str,
        "Approve",
        ui_approve_tx,
        "Reject",
        ui_reject_tx
    );
}

void ui_approve_tx(void) {
    uint8_t hash[32];
    cx_hash_sha256((uint8_t *)&G_honey_ctx.tx, sizeof(honey_tx_t), hash, 32);

    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x9000);
}

void ui_reject_tx(void) {
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x6985);
}
