#include "os.h"
#include "ux.h"
#include "honey.h"
#include "nbgl_use_case.h"
#include <stdio.h>

static char amount_str[40];
static char chain_str[20];
static char nonce_str[20];
static char address_str[50];

static nbgl_pageInfoLongPress_t approve_page;

/* ---------- FORMAT HELPERS ---------- */

static void format_amount(uint64_t raw) {
    // 18 decimals
    uint64_t whole = raw / 1000000000000000000ULL;
    uint64_t frac  = raw % 1000000000000000000ULL;

    snprintf(amount_str, sizeof(amount_str),
             "%llu.%018llu HNY",
             (unsigned long long)whole,
             (unsigned long long)frac);
}

static void format_address(uint8_t *addr) {
    snprintf(address_str, sizeof(address_str),
        "%02X%02X%02X%02X...%02X%02X%02X%02X",
        addr[0], addr[1], addr[2], addr[3],
        addr[16], addr[17], addr[18], addr[19]);
}

/* ---------- UI FLOW ---------- */

void ui_display_tx(void) {
    honey_tx_t *tx = &G_honey_ctx.tx;

    format_amount(tx->amount);
    format_address(tx->to);

    snprintf(chain_str, sizeof(chain_str), "%lu",
             (unsigned long)tx->chain_id);

    snprintf(nonce_str, sizeof(nonce_str), "%llu",
             (unsigned long long)tx->nonce[0]);

    nbgl_useCaseReviewStart(
        "Review Honey Tx",
        "Verify transaction details",
        ui_reject_tx
    );

    nbgl_useCaseReviewAddText("Amount", amount_str);
    nbgl_useCaseReviewAddText("Recipient", address_str);
    nbgl_useCaseReviewAddText("Chain ID", chain_str);
    nbgl_useCaseReviewAddText("Nonce", nonce_str);

    approve_page.text = "Sign transaction";
    approve_page.longPressText = "Hold to sign";
    approve_page.longPressCallback = ui_approve_tx;

    nbgl_useCaseReviewEnd(&approve_page);
}

void ui_show_blind_status(void) {
    nbgl_useCaseChoice(
        "Blind signing",
        "Unknown transaction data",
        "Proceed with caution",
        "Continue",
        ui_approve_tx,
        "Reject",
        ui_reject_tx
    );
}

void ui_approve_tx(void) {
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x9000);
}

void ui_reject_tx(void) {
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x6985);
}
