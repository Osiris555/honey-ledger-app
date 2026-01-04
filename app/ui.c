#include "ui.h"
#include "ux.h"
#include "os.h"
#include <string.h>
#include <stdio.h>

/* ---------- GLOBAL STATE ---------- */

static char line1[32];
static char line2[32];
static char line3[32];
static char line4[32];

static const honey_tx_t *pending_tx = NULL;

/* ---------- UX FLOW DECLARATIONS ---------- */

UX_STEP_NOCB(
    ux_idle_step,
    nn,
    {
        "Honey Ledger",
        "Ready"
    }
);

UX_FLOW(
    ux_idle_flow,
    &ux_idle_step
);

/* ---------- ADDRESS DISPLAY ---------- */

UX_STEP_NOCB(
    ux_address_step,
    nn,
    {
        "Your Address",
        line1
    }
);

UX_FLOW(
    ux_address_flow,
    &ux_address_step
);

/* ---------- TRANSACTION CONFIRMATION ---------- */

UX_STEP_NOCB(
    ux_tx_warning_step,
    nn,
    {
        "WARNING",
        "Review Tx"
    }
);

UX_STEP_NOCB(
    ux_tx_amount_step,
    nn,
    {
        "Amount",
        line1
    }
);

UX_STEP_NOCB(
    ux_tx_fee_step,
    nn,
    {
        "Fee",
        line2
    }
);

UX_STEP_NOCB(
    ux_tx_to_step,
    nn,
    {
        "To",
        line3
    }
);

UX_STEP_CB(
    ux_tx_approve_step,
    pb,
    {
        &C_icon_validate_14,
        "Approve",
    },
    {
        os_sched_exit(0);
    }
);

UX_STEP_CB(
    ux_tx_reject_step,
    pb,
    {
        &C_icon_crossmark,
        "Reject",
    },
    {
        THROW(0x6985); // User rejected
    }
);

UX_FLOW(
    ux_tx_flow,
    &ux_tx_warning_step,
    &ux_tx_amount_step,
    &ux_tx_fee_step,
    &ux_tx_to_step,
    &ux_tx_approve_step,
    &ux_tx_reject_step
);

/* ---------- UI FUNCTIONS ---------- */

void ui_idle(void) {
    ux_flow_init(0, ux_idle_flow, NULL);
}

void ui_show_address(const char *addr) {
    strncpy(line1, addr, sizeof(line1) - 1);
    line1[sizeof(line1) - 1] = 0;
    ux_flow_init(0, ux_address_flow, NULL);
}

void ui_confirm_tx(const honey_tx_t *tx) {

    pending_tx = tx;

    snprintf(line1, sizeof(line1), "%llu", (unsigned long long)tx->amount);
    snprintf(line2, sizeof(line2), "%llu", (unsigned long long)tx->fee);

    // Show first 8 bytes of destination
    snprintf(
        line3,
        sizeof(line3),
        "%02X%02X%02X%02X…",
        tx->to[0],
        tx->to[1],
        tx->to[2],
        tx->to[3]
    );

    ux_flow_init(0, ux_tx_flow, NULL);
}
