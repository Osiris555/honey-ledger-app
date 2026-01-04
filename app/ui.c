#include "ui.h"
#include "os.h"
#include "ux.h"

#include <string.h>
#include <stdio.h>

static honey_tx_t current_tx;
static volatile int ui_approved = 0;

static char addr_str[43];
static char amount_str[32];

/* ---------- Formatting helpers ---------- */

static void format_address(const uint8_t addr[20]) {
    snprintf(
        addr_str,
        sizeof(addr_str),
        "%02X%02X%02X%02X...%02X%02X",
        addr[0], addr[1], addr[2], addr[3],
        addr[18], addr[19]
    );
}

static void format_amount(const uint8_t amount[32]) {
    // Display last 18 decimals only (simple + safe for Nano S)
    uint64_t low = 0;
    memcpy(&low, amount, sizeof(uint64_t));

    uint64_t whole = low / 1000000000000000000ULL;
    uint64_t frac  = low % 1000000000000000000ULL;

    snprintf(
        amount_str,
        sizeof(amount_str),
        "%llu.%018llu HNY",
        (unsigned long long)whole,
        (unsigned long long)frac
    );
}

/* ---------- UI callbacks ---------- */

static void approve_cb(void) {
    ui_approved = 1;
    ui_idle();
}

static void reject_cb(void) {
    ui_approved = 0;
    ui_idle();
    THROW(0x6985); // User rejected
}

/* ---------- UI flow ---------- */

UX_STEP_NOCB(ux_tx_step_1,
    pnn,
    {
        .title = "Send",
        .text  = "HONEY?"
    }
);

UX_STEP_NOCB(ux_tx_step_2,
    bnnn_paging,
    {
        .title = "To",
        .text  = addr_str
    }
);

UX_STEP_NOCB(ux_tx_step_3,
    bnnn_paging,
    {
        .title = "Amount",
        .text  = amount_str
    }
);

UX_STEP_CB(ux_tx_step_4,
    pb,
    approve_cb(),
    {
        .title = "Approve",
        .text  = "Transaction"
    }
);

UX_STEP_CB(ux_tx_step_5,
    pb,
    reject_cb(),
    {
        .title = "Reject",
        .text  = "Transaction"
    }
);

UX_FLOW(ux_tx_flow,
    &ux_tx_step_1,
    &ux_tx_step_2,
    &ux_tx_step_3,
    &ux_tx_step_4,
    &ux_tx_step_5
);

/* ---------- Public API ---------- */

void ui_confirm_tx(const honey_tx_t *tx) {
    memcpy(&current_tx, tx, sizeof(honey_tx_t));

    format_address(current_tx.to);
    format_amount(current_tx.amount);

    ui_approved = 0;
    ux_flow_init(0, ux_tx_flow, NULL);

    while (!ui_approved) {
        io_seproxyhal_io_heartbeat();
    }
}

void ui_idle(void) {
    UX_IDLE();
}
