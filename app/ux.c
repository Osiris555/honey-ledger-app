#include "os.h"
#include "ux.h"
#include "honey.h"
#include "nbgl_use_case.h"
#include <stdio.h>

/* ---------- GLOBAL BUFFERS (NO STACK) ---------- */

static char g_amount[32];
static char g_chain[12];
static char g_nonce[12];
static char g_addr[42];

static nbgl_pageInfoLongPress_t g_approve_page;

/* ---------- FORMATTERS ---------- */

static void format_amount(uint64_t raw) {
    uint64_t whole = raw / 1000000000000000000ULL;
    uint64_t frac  = raw % 1000000000000000000ULL;

    snprintf(g_amount, sizeof(g_amount),
             "%llu.%018llu",
             (unsigned long long)whole,
             (unsigned long long)frac);
}

static void format_address(const uint8_t *a) {
    snprintf(g_addr, sizeof(g_addr),
        "%02X%02X%02X%02X…%02X%02X%02X%02X",
        a[0], a[1], a[2], a[3],
        a[16], a[17], a[18], a[19]);
}

/* ---------- UI FLOW ---------- */

void ui_display_tx(void) {
    honey_tx_t *tx = &G_honey_ctx.tx;

    format_amount(tx->amount);
    format_address(tx->to);

    snprintf(g_chain, sizeof(g_chain), "%lu",
             (unsigned long)tx->chain_id);

    snprintf(g_nonce, sizeof(g_nonce), "%u",
             (unsigned)tx->nonce[0]);

    nbgl_useCaseReviewStart(
        "Honey Tx",
        "Check details",
        ui_reject_tx
    );

    nbgl_useCaseReviewAddText("Amount", g_amount);
    nbgl_useCaseReviewAddText("To", g_addr);
    nbgl_useCaseReviewAddText("Chain", g_chain);
    nbgl_useCaseReviewAddText("Nonce", g_nonce);

    g_approve_page.text = "Sign";
    g_approve_page.longPressText = "Hold to sign";
    g_approve_page.longPressCallback = ui_approve_tx;

    nbgl_useCaseReviewEnd(&g_approve_page);
}

void ui_approve_tx(void) {
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x9000);
}

void ui_reject_tx(void) {
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x6985);
}
